private nosave string *tests = ({}), __Mode;
private nosave mapping __Tests = ([
    /* Data format:
    "/secure/module/test.test.c": ([
        "touched": 16000000,
        "ran": 16000000,
        "code": "/secure/module/test.c",
        "codeTouched": 160000000,
    ])
    */
]);
private nosave int currentTest = 0, totalTests = 0, totalFiles = 0;
private nosave int totalPassed = 0, totalFailed = 0, totalFnsTested = 0, totalFnsUntested = 0;
private nosave int timeBefore, timeAfter;
private nosave int shutdownAfterTests = 0;

// -----------------------------------------------------------------------------

void process ();
// void watch_all ();

// -----------------------------------------------------------------------------

varargs void done (int numTests, int numPassed, int numFailed, int fnsTested, int fnsUntested) {
    if (__Mode == "ALL") {
        totalTests += numTests;
        totalPassed += numPassed;
        totalFailed += numFailed;
        totalFnsTested += fnsTested;
        totalFnsUntested += fnsUntested;
        currentTest ++;
        process();
    } else if (__Mode == "WATCH") {
        totalTests = numTests;
        totalPassed = numPassed;
        totalFailed = numFailed;
        totalFnsTested = fnsTested;
        totalFnsUntested = fnsUntested;
    }
}

void process () {
    __Mode = "ALL";
    if (currentTest < sizeof(tests)) {
        object t;
        if (t = find_object(tests[currentTest])) destruct(t);
        if (!inherits(M_TEST, load_object(tests[currentTest]))) done();
        else {
            // call_out clears the call stack, call_other will chain the tests
            call_out(function() {
                mixed err = catch(tests[currentTest]->execute_test((: done :)));
                if (err) {
                    message("system", err + "\n", this_user());
                }
            }, 0);
        }
    } else {
        int totalExpects = totalPassed + totalFailed;
        timeAfter = rusage()["utime"] + rusage()["stime"];
        write("\nFiles:     " + sprintf("%3d", currentTest) + " / " + sprintf("%3d", totalFiles) + " (" + sprintf("%3d", to_int(currentTest * 100 / totalFiles)) + "%)"+"\n");
        write("Passed:    " + sprintf("%3d", totalPassed) + " / " + sprintf("%3d", totalExpects) + " (" + sprintf("%3d", to_int(totalPassed * 100 / totalExpects)) + "%)"+"\n");
        write("Failed:    " + sprintf("%3d", totalFailed) + " / " + sprintf("%3d", totalExpects) + " (" + sprintf("%3d", to_int(totalFailed * 100 / totalExpects)) + "%)"+"\n");
        write("Functions: " + sprintf("%3d", totalFnsTested) + " / " + sprintf("%3d", (totalFnsTested + totalFnsUntested)) + " (" + sprintf("%3d", to_int(totalFnsTested * 100 / (totalFnsTested + totalFnsUntested))) + "%)"+"\n");
        write("Time:      " + (timeAfter - timeBefore) + " ms for "+totalTests+" tests\n\n");
        // call_out((: watch_all :), 2, 0);
        if (shutdownAfterTests) {
            shutdown(totalFailed > 0 ? -1 : 0);
        }
    }
}

private void update_and_execute (string path) {
    call_other(path, "???");
    // call_out clears the call stack, call_other will chain the tests
    call_out(function() { tests[currentTest]->execute_test((: done :)); }, 0);
}


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
    mixed *dir = get_dir(path, -1); // Assumes path has trailing /
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
            __Tests[tmp]["codeTouched"] = file[2];
        } else {
            write("Missing " + path + file[0][0..<2] + "test.c"+"\n");
        }
    }
}

varargs void run (int callShutdown) {

    remove_call_out();
    currentTest = 0;
    totalTests = 0;
    totalFiles = 0;
    totalPassed = 0;
    totalFailed = 0;
    totalFnsTested = 0;
    totalFnsUntested = 0;
    __Tests = ([]);

    shutdownAfterTests = callShutdown;

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