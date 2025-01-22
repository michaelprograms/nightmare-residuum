inherit STD_COMMAND;

void create () {
    command::create();
    set_syntax("alias ([name] [value])|(remove [name])");
    set_help_text("The alias command is used to view your current alias list, or if a name is produced, create a new alias if possible, or if remove and a name is provided, attempt to delete an alias.\n\nAliases can be used as shortcuts for longer commands. For example, " + format_syntax("alias bag put $* in bag") + " will allow you to type " + format_syntax("bag [item]") + " instead of " + format_syntax("put [item] in bag") + ".");
}

/**
 * Prints all aliases that a user has set.
 *
 * @param {STD_USER} user the owner of the aliases
 */
private void print_all_aliases (object user) {
    mapping alias;
    string *aliasKeys;
    string *list = ({ });

    aliasKeys = user->query_alias_names();
    foreach (string name in sort_array(aliasKeys, 1)) {
        alias = user->query_alias(name);
        list += ({ name, alias["t"], });
        if (sizeof(alias["d"]) == 1 && alias["d"][0] != "") {
            list += ({ "$*: " + alias["d"][0], });
        } else if (sizeof(alias["d"]) > 1) {
            list += ({ "$0: " + alias["d"][0], });
            for (int i = 1; i < sizeof(alias["d"]); i ++) {
                // insert blank 1st & 2nd columns
                list += ({ "", "", "$" + i + ": " + alias["d"][i], });
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
    object tu = this_user();
    if (!input) {
        print_all_aliases(tu);
    } else {
        string *argv = explode(input, " ");
        string name;

        if (sizeof(argv) == 1) {
            mapping alias;
            alias = tu->query_alias(argv[0]);
            if (!alias) {
                message("action", "Alias '" + argv[0] + "': not found", tu);
            } else {
                message("action", "Alias '" + argv[0] + "': " + alias["t"], tu);
            }
        } else {
            name = implode(argv[1..], " ");
            message("action", "Adding alias: '" + argv[0] + "' as '" + name + "'", tu);
            tu->set_alias(argv[0], name);
        }
    }
}