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
    ob->handle_move(this_character());
    write("You clone " + ob->query_name() + ".\n");
}
