varargs void done (int numTests, int numPassed, int numFailed, int fnsTested, int fnsUntested) {
    int numExpects = numPassed + numFailed;
    if (!numExpects) {
        message("system", "\nNo expects found.\n", this_user());
        return;
    }
    message("system", "\nPassed:    " + sprintf("%3d", numPassed) + " / " + sprintf("%3d", numExpects) + " (" + sprintf("%3d", to_int(numPassed * 100 / numExpects)) + "%)"+"\n", this_user());
    message("system", "Failed:    " + sprintf("%3d", numFailed) + " / " + sprintf("%3d", numExpects) + " (" + sprintf("%3d", to_int(numFailed * 100 / numExpects)) + "%)"+"\n", this_user());
    message("system", "Functions: " + sprintf("%3d", fnsTested) + " / " + sprintf("%3d", (fnsTested + fnsUntested)) + " (" + sprintf("%3d", to_int(fnsTested * 100 / (fnsTested + fnsUntested))) + "%)"+"\n\n", this_user());
}

void command (string input) {
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
            D_TEST->process_file(test, (: done :), 1);
        } else {
            message("system", "Unable to find test file for " + input + ".\n", this_user());
        }
    } else {
        message("system", "Test what?\n", this_user());
    }
}