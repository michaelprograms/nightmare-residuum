void command (string input, mapping flags) {
    string path;
    object file;
    string *border;
    string result, *items;

    if (!input) {
        message("action", "Syntax: help [command]\n", this_character());
        return;
    }

    if (path = D_COMMAND->query_ability(input)) {
        path += "/" + input + ".c";
        file = find_object(path) || load_object(path);
        if (function_exists("help", file)) {
            result = call_other(path, "help", this_character());
        }
    }
    if (path = D_COMMAND->query_verb(input)) {
        path += "/" + input + ".c";
        file = find_object(path) || load_object(path);
        if (function_exists("help", file)) {
            result = call_other(path, "help", this_character());
        }
    }

    if (sizeof(result)) {
        items = explode(result, "\n");
    }
    border = format_border(([
        "title": "HELP",
        "subtitle": lower_case(input),
        "body": ([
            "items": sizeof(items) ? items : ({ "No help page found for that topic." }),
            "columns": 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}