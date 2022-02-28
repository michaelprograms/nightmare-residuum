void command (string input) {
    string cwd, file;
    object ob;

    if (!input) {
        write("Syntax: clone [file]\n");
        return;
    }
    cwd = this_user()->query_shell()->query_variable("cwd");
    file = absolute_path(input, cwd);
    ob = clone_object(file);
    if (ob->is_living()) {
        write("You clone " + ob->query_name() + " into your environment.\n");
        ob->handle_move(environment(this_character()));
    } else {
        write("You clone " + ob->query_name() + " into your inventory.\n");
        ob->handle_move(this_character());
    }
}
