inherit STD_COMMAND;

void create() {
    ::create();
    set_syntax("pwd");
    set_help_text("The pwd command is used to print an immortal's working directory.");
}

void command(string _input, mapping _flags) {
    write("Current working directory: " + this_user()->query_variable("cwd") + "\n");
}
