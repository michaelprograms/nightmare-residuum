void done (mapping results) {
    int numExpects = results["numPassed"] + results["numFailed"];
    if (!numExpects) {
        message("system", "\nNo expects found.\n", this_user());
        return;
    }
    message("system", "\nPassed:    " + sprintf("%3d", results["numPassed"]) + " / " + sprintf("%3d", numExpects) + " (" + sprintf("%3d", to_int(results["numPassed"] * 100 / numExpects)) + "%)"+"\n", this_user());
    message("system", "Failed:    " + sprintf("%3d", results["numFailed"]) + " / " + sprintf("%3d", numExpects) + " (" + sprintf("%3d", to_int(results["numFailed"] * 100 / numExpects)) + "%)"+"\n", this_user());
    message("system", "Functions: " + sprintf("%3d", results["fnsTested"]) + " / " + sprintf("%3d", (results["fnsTested"] + results["fnsUntested"])) + " (" + sprintf("%3d", to_int(results["fnsTested"] * 100 / (results["fnsTested"] + results["fnsUntested"]))) + "%)"+"\n\n", this_user());
    if (strlen(results["failingExpects"]) > 0) {
        message("system","Failing expects:\n" + results["failingExpects"] + "\n\n", this_user());
    }
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