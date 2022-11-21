inherit STD_COMMAND;

void create () {
    set_syntax("test [file]");
    set_help_text("The test command is used to run the tests of a file, if they exist.");
}

nosave private int testStart;
void done (mapping results) {
    D_TEST->display_results(results, testStart);
}

void command (string input, mapping flags) {
    if (input == "all") {
        message("system", "Running all tests...\n", this_user());
        D_TEST->run(0);
    } else if (input) {
        string test = input;
        if (input[<2..] == ".c") {
            input = input[0..<3];
        }
        test = input + ".test.c";
        if (file_size(test) > 0) {
            testStart = perf_counter_ns();
            D_TEST->process_file(test, (: done :), 1);
        } else {
            message("system", "Unable to find test file for " + input + ".\n", this_user());
        }
    } else {
        message("system", "Test what?\n", this_user());
    }
}