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

// -----------------------------------------------------------------------------

void process_all ();
void watch_all ();

// -----------------------------------------------------------------------------

varargs void done (int numTests, int numPassed, int numFailed, int fnsTested, int fnsUntested) {
    if (__Mode == "ALL") {
        totalTests += numTests;
        totalPassed += numPassed;
        totalFailed += numFailed;
        totalFnsTested += fnsTested;
        totalFnsUntested += fnsUntested;
        currentTest ++;
        process_all();
    } else if (__Mode == "WATCH") {
        totalTests = numTests;
        totalPassed = numPassed;
        totalFailed = numFailed;
        totalFnsTested = fnsTested;
        totalFnsUntested = fnsUntested;
    }
}

void process_all () {
    __Mode = "ALL";
    if (currentTest < sizeof(tests)) {
        if (!inherits(M_TEST, load_object(tests[currentTest]))) done();
        else {
            // call_out clears the call stack, call_other will chain the tests
            call_out(function() { tests[currentTest]->execute_test((: done :)); }, 0);
        }
    } else {
        timeAfter = rusage()["utime"] + rusage()["stime"];
        write("  Files:     " + currentTest + " / " + totalFiles + " (" + to_int(currentTest * 100 / totalFiles) + "%)"+"\n");
        write("  Tests:     " + totalTests+"\n");
        write("  Passed:    " + totalPassed + " / " + (totalPassed + totalFailed) + " (" + to_int(totalPassed * 100 / (totalPassed + totalFailed)) + "%)"+"\n");
        write("  Failed:    " + totalFailed + " / " + (totalPassed + totalFailed) + " (" + to_int(totalFailed * 100 / (totalPassed + totalFailed)) + "%)"+"\n");
        write("  Functions: " + totalFnsTested + " / " + (totalFnsTested + totalFnsUntested) + " (" + to_int(totalFnsTested * 100 / (totalFnsTested + totalFnsUntested)) + "%)"+"\n");
        write("  Time:      " + (timeAfter - timeBefore) + " ms"+"\n");
        // call_out((: watch_all :), 2, 0);
    }
}

private void update_and_execute (string path) {
    call_other(path, "???");
    // call_out clears the call stack, call_other will chain the tests
    call_out(function() { tests[currentTest]->execute_test((: done :)); }, 0);
}


void watch_all () {
    int flag, touched;
    string path;
    object testOb;

    __Mode = "WATCH";
    for (int i = 0; i < sizeof(tests); i ++) {
        flag = 0;
        path = tests[i];
        touched = stat(path)[1];
        if (__Tests[path]["touched"] < touched) {
            __Tests[path]["touched"] = touched;
            flag = 1;
        }
        touched = stat(__Tests[path]["code"])[1];
        if (__Tests[path]["codeTouched"] < touched) {
            __Tests[path]["codeTouched"] = touched;
            flag = 1;
        }
        if (flag) {
            write("\n" + path + " has been changed..."+"\n");
            totalTests = totalPassed = totalFailed = totalFnsTested = totalFnsUntested = 0;
            currentTest = i;
            timeBefore = rusage()["utime"] + rusage()["stime"];

            if (testOb = find_object(path)) destruct(testOb);
            catch (update_and_execute(path));
            timeAfter = rusage()["utime"] + rusage()["stime"];
        }
    }

    call_out((: watch_all :), 2, 0);
}

void update_test_data (string path) {
    mixed *dir = get_dir(path, -1); // Assumes path has trailing /
    string *codeFiles = ({}), tmp;

    foreach (mixed *file in dir) {
        if (file[1] == -2) {
            update_test_data(path + file[0] + "/");
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

void run () {
    // remove_call_out();
    // currentTest = 0;
    // totalTests = 0;
    // totalFiles = 0;
    // totalPassed = 0;
    // totalFailed = 0;
    // totalFnsTested = 0;
    // totalFnsUntested = 0;
    // __Tests = ([]);
    update_test_data("/secure/");
    update_test_data("/daemon/");
    update_test_data("/std/");
    write(""+"\n");

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
    call_out((: process_all :), 0);
}