void command (string input) {
    string cwd, file;
    object ob;

    if (!input) {
        write("Syntax: clone [file]\n");
        return;
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    if (file_size(file) < 1) {
        write("clone: no such file.\n");
        return;
    }

    ob = clone_object(file);
    if (ob->is_living()) {
        message("action", "You clone " + ob->query_name() + " into your environment.\n", this_character());
        message("action", this_character()->query_name() + " clones " + ob->query_name() + ".\n", environment(this_character()), this_character());
        ob->handle_move(environment(this_character()));
    } else {
        message("action", "You clone " + ob->query_name() + " into your inventory.\n", this_character());
        message("action", this_character()->query_name() + " clones " + ob->query_name() + ".\n", environment(this_character()), this_character());
        ob->handle_move(this_character());
    }
}
