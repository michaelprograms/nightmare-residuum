void command (string input) {
    object shell = this_user()->query_shell();

    write("input is: '"+input+"'.\n");
    if (!input) {
        write("Prompt is currently set to '" + shell->query_variable("prompt") + "'.\n");
    } else {
        write("Changing prompt from '" + shell->query_variable("prompt") + "' to '" + input + "'.\n");
        shell->set_variable("prompt", input);
    }
}