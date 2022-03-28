nosave private mapping __Tests = ([
    /* Data format:
    "/std/module/test.test.c": ([
        "touched": 16000000,
        "ran": 16000000,
        "code": "/std/module/test.c",
        "codeTouched": 160000000,
    ])
    */
]);
nosave private string *__TestFiles = ({});
nosave private mapping __Results = ([]);

nosave private int currentTest = 0, shutdownAfterTests = 0;
nosave private int totalFiles = 0;

nosave private int timeBefore;

// -----------------------------------------------------------------------------

void reset_data ();
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

    __Results["numTests"] = 0;
    __Results["passingExpects"] = 0;
    __Results["failingExpects"] = 0;
    __Results["testedFns"] = 0;
    __Results["untestedFns"] = 0;
    __Results["failLog"] = ({ });
    __Results["passingAsserts"] = 0;
    __Results["failingAsserts"] = 0;

    totalFiles = 0;
    __Tests = ([ ]);
    __TestFiles = ({ });
}

// -----------------------------------------------------------------------------

void done_test (mapping results) {
    if (results) {
        __Results["numTests"] += results["numTests"];
        __Results["passingExpects"] += results["passingExpects"];
        __Results["failingExpects"] += results["failingExpects"];
        __Results["testedFns"] += results["testedFns"];
        __Results["untestedFns"] += results["untestedFns"];
        __Results["passingAsserts"] += results["passingAsserts"];
        __Results["failingAsserts"] += results["failingAsserts"];
        if (strlen(results["failLog"]) > 0) {
            __Results["failLog"] += ({ results["failLog"] });
        }
    }
    currentTest ++;
    process();
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
        return;
    }
    if (!inherits(M_TEST, load_object(file))) {
        evaluate(doneCallback);
        return;
    }
    // call out clears the call stack, call other will chain the tests
    call_out_walltime(function(string test, function doneCallback) {
        object testFile = clone_object(test);
        mixed err = catch(testFile->execute_test(doneCallback));
        if (err) {
            write("\n    " + test + " encountered an errored:\n" + err + "\n");
            evaluate(doneCallback);
        }
        if (testFile) destruct(testFile);
    }, 0, file, doneCallback);
}

private string format_total_line (string name, int current, int total) {
    string tmp = sprintf("%-20s", name + ":");
    tmp += sprintf("%3d", current) + " / " + sprintf("%-3d", total);
    tmp += "  (" + sprintf("%6.2f", current * 100.0 / total) + "%)";
    return tmp;
}

void display_results (mapping results, int timeStart) {
    int totalExpects = results["passingExpects"] + results["failingExpects"];
    int totalAsserts = results["passingAsserts"] + results["failingAsserts"];
    int totalFns = results["testedFns"] + results["untestedFns"];
    int time;

    if (!undefinedp(timeStart)) {
        time = (rusage()["utime"] + rusage()["stime"]) - timeStart;
    }

    write("\n");
    if (totalFiles > 1) {
        write(format_total_line("Test Files", currentTest, totalFiles) + "\n");
    }
    write(format_total_line("Functions", results["testedFns"], totalFns) + "\n");
    write(format_total_line("Expects Passed", results["passingExpects"], totalExpects) + "\n");
    write(format_total_line("Asserts Passed", results["passingAsserts"], totalAsserts) + "\n");

    if (!undefinedp(timeStart)) {
        write("\n" + sprintf("%-20s", "Time" + ":") + time + " ms for " + results["numTests"] + " tests\n\n");
    }

    if (sizeof(results["failLog"]) > 0) {
        write("Failing expects:\n" + implode(results["failLog"], "\n") + "\n\n");
        results["failLog"] = ({});
    }
}

void process () {
    if (currentTest < sizeof(__TestFiles)) {
        process_file(__TestFiles[currentTest], (: done_test :), 0);
    } else {
        display_results(__Results, timeBefore);

        if (shutdownAfterTests) {
            shutdown(__Results["failingExpects"] > 0 ? -1 : 0);
        }
    }
}

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

    __TestFiles = keys(__Tests);
    __TestFiles = sort_array(__TestFiles, function(string a, string b) {
        if (regexp(a, "^\\/std\\/module\\/test")) return -1;
        else if (regexp(b, "^\\/std\\/module\\/test")) return 1;
        else if (regexp(a, "^\\/secure\\/daemon\\/master")) return -1;
        else if (regexp(b, "^\\/secure\\/daemon\\/master")) return 1;
        else if (regexp(a, "^\\/secure\\/sefun\\/sefun")) return -1;
        else if (regexp(b, "^\\/secure\\/sefun\\/sefun")) return 1;
        else if (regexp(a, "^\\/secure\\/sefun\\/")) return -1;
        else if (regexp(b, "^\\/secure\\/sefun\\/")) return 1;
        else return strcmp(a, b);
    });

    timeBefore = rusage()["utime"] + rusage()["stime"];
    call_out_walltime((: process :), 0);
}