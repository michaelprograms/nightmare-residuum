void command (string input) {
    mapping actions = ([]);

    write(format_header_bar("ACTIONS") + "\n");

    if (input == "rescan") {
        D_COMMAND->scan_all();
        write("\n%^BOLD%^Commands and verbs rescanned.%^RESET%^\n");
    }

    foreach (string path in D_COMMAND->query_paths()) {
        string type = split_path(path)[<1];
        if (!actions[type]) actions[type] = ({});
        if (type == "immortal" || type == "character") {
            foreach (string action in D_COMMAND->query_commands(path)) {
                actions[type] += ({ action });
            }
        } else if (type == "verb") {
            foreach (string action in D_COMMAND->query_verbs(path)) {
                actions[type] += ({ action });
            }
        }
    }
    foreach (string type in keys(actions)) {
        string *line = ({});
        write("\n%^BOLD%^" + type + "%^RESET%^\n");
        foreach(string a in actions[type]) {
            line += ({ a });
        }
        line = sort_array(line, 1);
        write(implode(line, ", ")+"\n");
    }
    write("\n" + format_footer_bar() + "\n");
}