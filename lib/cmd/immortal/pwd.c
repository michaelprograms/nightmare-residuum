void command (string input, mapping flags) {
    write("Current working directory: "+this_user()->query_shell()->query_variable("cwd")+"\n");
}