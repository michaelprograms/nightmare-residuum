inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("goto [character|room path]");
    set_help_text("The goto command can be used by an immortal to go to another character's environment or a specified room path.");
}

void command (string input, mapping flags) {
    object ob;
    string path;

    if (!input) {
        message("action", "Syntax: goto [character|room path]", this_character());
        return;
    }

    if (
        (ob = find_character(input)) ||
        (regexp(input, "#") && (ob = find_object(input)))
    ) {
        message("action", "Going to "+input+"...", this_character());
        this_character()->handle_go(environment(ob), "teleport", "away");
        return;
    } else {
        path = input;
        if (path[<2..] != ".c") {
            path += ".c";
        }
        if (!regexp(path, "/virtual/")) {
            if (file_size(path) == -1) {
                path = this_user()->query_variable("cwd") + "/" + path;
            }
            if (file_size(path) == -1) {
                message("action", "goto: " + input + " not found.", this_character());
                return;
            }
        }

        if (this_character()->query_immortal()) {
            message("action", "Going to "+path+"...", this_character());
        } else {
            message("action", "Going...", this_character());
        }
        this_character()->handle_go(path, "teleport", "away");
    }
}