inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("test [file]");
    set_help_text("The test command is used to run the tests of a file, if they exist.");
}

nosave private int testStart;
void done (mapping results) {
    D_TEST->display_results(results, testStart);
}

void command (string input, mapping flags) {
    if (input == "all") {
        message("action", "Running all tests...", this_user());
        D_TEST->run(0);
    } else if (input) {
        string test = input;
        if (input[<2..] == ".c") {
            input = input[0..<3];
        }
        test = input + ".test.c";
        if (file_size(test) > 0) {
            testStart = time_ns();
            D_TEST->process_file(test, (: done :), 1);
        } else {
            message("action", "Unable to find test file for " + input + ".", this_user());
        }
    } else {
        message("action", "Test what?", this_user());
    }
}