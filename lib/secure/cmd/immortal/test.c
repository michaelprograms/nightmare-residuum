inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("test [file]");
    set_help_text("The test command is used to run the tests of a file, if they exist.");
}

void command (string input, mapping flags) {
    if (input == "all") {
        message("action", "Running all tests...", this_user());
        D_TEST->run(0);
    } else if (input) {
        string test = input;

        if (!input || input == "") {
            message("action", "Update which file?", this_user());
            return;
        } else if (input[0] != '/' && input[0] != '~' && input[0] != '^') {
            input = this_user()->query_shell()->query_variable("cwd") + "/" + input;
        }
        input = sanitize_path(input);
        if (input[<2..] == ".c") {
            input = input[0..<3];
        }
        test = input + ".test.c";
        if (file_size(test) > 0) {
            D_TEST->process_file(test, ([ "reset": 1 ]));
        } else {
            message("action", "Unable to find test file for " + input + ".", this_user());
        }
    } else {
        message("action", "Test what?", this_user());
    }
}