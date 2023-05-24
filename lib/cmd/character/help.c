inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("help [command]");
    set_help_text("The help command is used to view more information about a command.");
}

void command (string input, mapping flags) {
    string path;
    object ob;
    string result, *parts, *items;

    if (!input) {
        message("action", "Syntax: help [command]", this_character());
        return;
    }

    if (
        (path = D_COMMAND->query_command(input)) ||
        (path = D_COMMAND->query_verb(input)) ||
        (path = D_COMMAND->query_ability(input))
    ) {
        path += "/" + input + ".c";
        ob = find_object(path) || load_object(path);
        if (function_exists("handle_help", ob)) {
            result = call_other(path, "handle_help", this_character());
        }
    } else {
        path = "/doc/help/" + input;
        if (file_size(path) > 0) {
            result = read_file(path);
            parts = explode(result, ">");
            result = "";
            parts[0..<2] = map(parts[0..<2], (: $1 + ">" :));
            foreach (string part in parts) {
                result += pcre_replace_callback(part, "(<[a-z]+\\s?.+>)", (: format_syntax($1) :));
            }
        }
    }

    if (sizeof(result)) {
        items = explode(result, "\n");
    } else {
        items = ({ "No help page found for that topic." });
    }

    border(([
        "title": "HELP",
        "subtitle": lower_case(input),
        "body": ([
            "items": items,
            "columns": 1,
            "align": sizeof(items) ? "left" : "center",
        ]),
    ]));
}