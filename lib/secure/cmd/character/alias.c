inherit "/std/class/shell_alias.c";
inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("alias ([name] [value])|(remove [name])");
    set_help_text("The alias command is used to view your current alias list, or if a name is produced, create a new alias if possible, or if remove and a name is provided, attempt to delete an alias.\n\nAliases can be used as shortcuts for longer commands. For example, " + format_syntax("alias bag put $* in bag") + " will allow you to type " + format_syntax("bag [item]") + " instead of " + format_syntax("put [item] in bag") + ".");
}

private void print_all_aliases (object shell) {
    class ShellAlias alias;
    string *aliasKeys;
    string *list = ({ });

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

    border(([
        "title": "ALIAS",
        "subtitle": "List",
        "body": ([
            "header": ({ "Alias", "Expansion", "Default", }),
            "items": list,
            "columns": ({ 1, 3, 2, }),
        ]),
        "footer": ([
            "items": ({
                sizeof(aliasKeys) + " aliases",
            }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
}

void command (string input, mapping flags) {
    if (!input) {
        print_all_aliases(this_user()->query_shell());
    } else {
        string *argv = explode(input, " ");
        string name;

        if (sizeof(argv) == 1) {
            class ShellAlias alias;
            alias = this_user()->query_shell()->query_alias(argv[0]);

            message("system", "Alias: " + argv[0] + ": " + alias->template + ".\n", this_user());
            return;
        }
        name = implode(argv[1..], " ");
        message("system", "Adding alias: '" + argv[0] + "' as '" + name + "'.\n", this_user());
        this_user()->query_shell()->add_alias(argv[0], name);
    }
}