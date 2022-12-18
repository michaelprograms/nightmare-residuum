void command (string input, mapping flags) {
    string cwd, file;
    object ob;

    if (!input) {
        message("action", "Syntax: clone [file]", this_character());
        return;
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    if (file_size(file) < 1) {
        message("action", "clone: no such file.", this_character());
        return;
    }

    ob = clone_object(file);
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
