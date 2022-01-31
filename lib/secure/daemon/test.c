nosave private string *tests = ({}), __Mode;
nosave private mapping __Tests = ([
    /* Data format:
    "/secure/module/test.test.c": ([
        "touched": 16000000,
        "ran": 16000000,
        "code": "/secure/module/test.c",
        "codeTouched": 160000000,
    ])
    */
]);
nosave private int currentTest = 0, shutdownAfterTests = 0;
nosave private int totalTests = 0, totalFiles = 0;
nosave private int totalPassed = 0, totalFailed = 0;
nosave private int totalFnsTested = 0, totalFnsUntested = 0;
nosave private int timeBefore, timeAfter;
nosave private string *failingExpects = ({});

// -----------------------------------------------------------------------------

void reset_data ();
varargs void done (int numTests, int numPassed, int numFailed, int fnsTested, int fnsUntested);
varargs void process_file (string file, function done, int reset);
private string format_total_line (string name, int current, int total);
void process ();
// private void update_and_execute (string path);
// void watch_all ();
varargs void update_test_data (string path, string ignore);
varargs void run (int callShutdown);

// -----------------------------------------------------------------------------

void reset_data () {
    currentTest = 0;
    totalTests = 0;
    totalFiles = 0;
    totalPassed = 0;
    totalFailed = 0;
    totalFnsTested = 0;
    totalFnsUntested = 0;
    __Tests = ([]);
    tests = ({});
}

// -----------------------------------------------------------------------------

void done (mapping results) {
    if (__Mode == "ALL") {
        if (results) {
            totalTests += results["numTests"];
            totalPassed += results["numPassed"];
            totalFailed += results["numFailed"];
            totalFnsTested += results["fnsTested"];
            totalFnsUntested += results["fnsUntested"];
            if (strlen(results["failingExpects"]) > 0) {
                failingExpects += ({ results["failingExpects"] });
            }
        }
        currentTest ++;
        process();
    // } else if (__Mode == "WATCH") {
    //     totalTests = results["numTests"];
    //     totalPassed = results["numPassed"];
    //     totalFailed = results["numFailed"];
    //     totalFnsTested = results["fnsTested"];
    //     totalFnsUntested = results["fnsUntested"];
    }
}

varargs void process_file (string file, function doneCallback, int reset) {
    object t;
    string tmp;

    if (reset) {
        reset_data();
        update_test_data(file);
    }
    if (t = find_object(file)) {
        destruct(t);
    }
    tmp = catch (call_other(file, "???"));
    if (tmp) {
        message("system", "Error in test: " + tmp + "\n", this_user());
    }
    if (!inherits(M_TEST, load_object(file))) {
        evaluate(doneCallback);
        return;
    }
    // call_out clears the call stack, call_other will chain the tests
    call_out_walltime(function(string test, function doneCallback) {
        mixed err = catch(test->execute_test(doneCallback));
        if (err) {
            write("\n    " + test + " encountered an errored:\n" + err + "\n");
            evaluate(doneCallback);
        }
    }, 0, file, doneCallback);
}

private string format_total_line (string name, int current, int total) {
    string tmp = sprintf("%-12s", name + ":");
    tmp += sprintf("%3d", current) + " / " + sprintf("%-3d", total);
    tmp += " (" + sprintf("%6.2f", current * 100.0 / total) + "%)";
    return tmp;
}

void process () {
    __Mode = "ALL";
    if (currentTest < sizeof(tests)) {
        process_file(tests[currentTest], (: done :), 0);
    } else {
        int totalExpects = totalPassed + totalFailed;
        timeAfter = rusage()["utime"] + rusage()["stime"];
        write("\n" + format_total_line("Files", currentTest, totalFiles) + "\n");
        write(format_total_line("Passed", totalPassed, totalExpects) + "\n");
        write(format_total_line("Failed", totalFailed, totalExpects) + "\n");
        write(format_total_line("Functions", totalFnsTested, totalFnsTested + totalFnsUntested) + "\n");
        write("Time:      " + (timeAfter - timeBefore) + " ms for " + totalTests + " tests\n\n");
        if (sizeof(failingExpects) > 0) {
            write("Failing expects:\n" + implode(failingExpects, "\n") + "\n\n");
            failingExpects = ({});
        }
        // call_out((: watch_all :), 2, 0);
        if (shutdownAfterTests) {
            shutdown(totalFailed > 0 ? -1 : 0);
        }
    }
}

// private void update_and_execute (string path) {
//     call_other(path, "???");
//     // call_out clears the call stack, call_other will chain the tests
//     call_out(function() { tests[currentTest]->execute_test((: done :)); }, 0);
// }


// void watch_all () {
//     int flag, touched;
//     string path;
//     object testOb;

//     __Mode = "WATCH";
//     for (int i = 0; i < sizeof(tests); i ++) {
//         flag = 0;
//         path = tests[i];
//         touched = stat(path)[1];
//         if (__Tests[path]["touched"] < touched) {
//             __Tests[path]["touched"] = touched;
//             flag = 1;
//         }
//         touched = stat(__Tests[path]["code"])[1];
//         if (__Tests[path]["codeTouched"] < touched) {
//             __Tests[path]["codeTouched"] = touched;
//             flag = 1;
//         }
//         if (flag) {
//             write("\n" + path + " has been changed..."+"\n");
//             totalTests = totalPassed = totalFailed = totalFnsTested = totalFnsUntested = 0;
//             currentTest = i;
//             timeBefore = rusage()["utime"] + rusage()["stime"];

//             if (testOb = find_object(path)) destruct(testOb);
//             catch (update_and_execute(path));
//             timeAfter = rusage()["utime"] + rusage()["stime"];
//         }
//     }

//     call_out((: watch_all :), 2, 0);
// }

varargs void update_test_data (string path, string ignore) {
    mixed *dir = get_dir(path, -1); // Assumes path has trailing / for dirs
    string *codeFiles = ({}), tmp;

    foreach (mixed *file in dir) {
        if (path + file[0] == ignore) continue;
        if (file[1] == -2) {
            update_test_data(path + file[0] + "/", ignore);
        } else if (regexp(file[0], "\\.test\\.c$")) {
            __Tests[path+file[0]] = ([
                "touched": file[2],
            ]);
        } else if (regexp(file[0], "\\.c$")) {
            totalFiles ++;
            codeFiles += ({ file });
        }
    }
    foreach (mixed *file in codeFiles) {
        tmp = path + replace_string(file[0], ".c", ".test.c", 1);
        if (__Tests[tmp]) {
            __Tests[tmp]["code"] = path + file[0];
            // __Tests[tmp]["codeTouched"] = file[2];
        } else {
            write("Missing " + path + file[0][0..<2] + "test.c"+"\n");
        }
    }
}

varargs void run (int callShutdown) {

    shutdownAfterTests = callShutdown;

    remove_call_out();
    reset_data();

    write("Scanning for test files...\n");
    update_test_data("/secure/", "/secure/cmd");
    update_test_data("/daemon/");
    update_test_data("/std/", "/std/class");

    tests = keys(__Tests);
    tests = sort_array(tests, function(string a, string b) {
        if (regexp(a, "^\\/secure\\/module\\/test")) return -1;
        else if (regexp(b, "^\\/secure\\/module\\/test")) return 1;
        else if (regexp(a, "^\\/secure\\/master\\/master")) return -1;
        else if (regexp(b, "^\\/secure\\/master\\/master")) return 1;
        else if (regexp(a, "^\\/secure\\/master\\/")) return -1;
        else if (regexp(b, "^\\/secure\\/master\\/")) return 1;
        else if (regexp(a, "^\\/secure\\/sefun\\/sefun")) return -1;
        else if (regexp(b, "^\\/secure\\/sefun\\/sefun")) return 1;
        else if (regexp(a, "^\\/secure\\/sefun\\/")) return -1;
        else if (regexp(b, "^\\/secure\\/sefun\\/")) return 1;
        else return strcmp(a, b);
    });

    timeBefore = rusage()["utime"] + rusage()["stime"];
    call_out((: process :), 0);
}