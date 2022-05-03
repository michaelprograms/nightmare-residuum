inherit "/std/class/shell_alias.c";

private void print_all_aliases (object shell) {
    class ShellAlias alias;
    string *aliasKeys;
    string *border, *list = ({ });

    aliasKeys = shell->query_alias_names();
    foreach (string name in sort_array(aliasKeys, 1)) {
        alias = shell->query_alias(name);
        list += ({ name, alias->template, });
        if (sizeof(alias->defaults) == 1 && alias->defaults[0] != "") {
            list += ({ "$*: " + alias->defaults[0], });
        } else if (sizeof(alias->defaults) > 1) {
            list += ({ "$0: " + alias->defaults[0], });
            for (int i = 1; i < sizeof(alias->defaults); i ++) {
                // insert blank 1st & 2nd columns
                list += ({ "", "", "$" + i + ": " + alias->defaults[i], });
            }
        } else {
            // insert blank 3rd column
            list += ({ "", });
        }
    }

    border = format_border(([
        "title": "ALIAS",
        "subtitle": "List",
        "body": ([
            "header": ({ "Alias", "Expansion", "Default", }),
            "items": list,
            "columns": 3,
        ]),
        "footer": ([
            "items": ({
                sizeof(aliasKeys) + " aliases",
            }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}

void command (string input, mapping flags) {
    if (!input) {
        print_all_aliases(this_user()->query_shell());
    } else {
        string *argv = explode(input, " ");
        string alias = implode(argv[1..], " ");
        write("Adding alias: " + argv[0] + " as " + alias + "\n");
        this_user()->query_shell()->add_alias(argv[0], alias);
    }
}