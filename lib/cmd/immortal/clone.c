inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("clone [file]");
    set_help_text("The clone command is used to create a copy of the object in the provided file.");
}

void command (string input, mapping flags) {
    string cwd, file;
    object ob;

    if (!input) {
        message("action", "Syntax: clone [file]", this_character());
        return;
    }
    cwd = this_user()->query_variable("cwd");
    file = absolute_path(input, cwd);
    if (file_size(file) < 1) {
        message("action", "clone: no such file.", this_character());
        return;
    }

    ob = new(file);
    if (ob->is_living() || ob->query_no_get()) {
        message("action", "You clone " + ob->query_cap_name() + " into your environment.", this_character());
        message("action", this_character()->query_cap_name() + " clones " + ob->query_cap_name() + ".", environment(this_character()), this_character());
        ob->handle_move(environment(this_character()));
    } else {
        message("action", "You clone " + ob->query_name() + " into your inventory.", this_character());
        message("action", this_character()->query_cap_name() + " clones " + ob->query_name() + ".", environment(this_character()), this_character());
        ob->handle_move(this_character());
    }
}
