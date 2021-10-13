#define ANSI(p) "\e["+(p)+"m"
#define RESET ANSI(0)
#define BOLD ANSI(1)
#define UNDERLINE ANSI(4)
#define RED ANSI(31)
#define GREEN ANSI(32)
#define ORANGE ANSI(33)
#define CYAN ANSI(36)

nosave protected mixed UNDEFINED = (([])[0]); // equivalent of UNDEFINED

// -----------------------------------------------------------------------------

private nosave int currentTestPassed = 0, currentTestRegex = 0;
private nosave int failingExpects = 0, passingExpects = 0;
private nosave string currentTestLog;

// -----------------------------------------------------------------------------

void before_all_tests () { }
void before_each_test () { }
void after_each_test () { }
void after_all_tests () { }
string *test_order () { return 0; }
string *test_ignore () {
    return ({
        "before_all_tests",
        "before_each_test",
        "after_each_test",
        "after_all_tests",
        "test_order",
        "test_ignore",
        "clone_object",
        "execute_test",
        "create",
        "init",
        "reset",
    });
}
protected expect_next_failure () {
    if (base_name(this_object()) == replace_string(M_TEST, ".c", ".test") && failingExpects == 0) {
        failingExpects --;
    }
}

// -----------------------------------------------------------------------------

string *testObjectFns = ({ }), *testObjectUntestedFns = ({ });

public int execute_test (function done) {
    string *testFns, *otherTestFns;
    int timeBefore, timeAfter;
    int failingExpectsBefore, passingExpectsBefore;

    if (sizeof(testFns = test_order()) == 0) {
        testFns = functions(this_object(), 2) - test_ignore();
    } else if (sizeof(testFns) != sizeof(functions(this_object(), 2))) {
        otherTestFns = functions(this_object(), 2) - test_ignore() - testFns;
        testFns += otherTestFns;
    }

    failingExpects = 0;
    passingExpects = 0;

    write("\nEvaluating '" + CYAN + UNDERLINE + file_name(this_object()) + RESET + "'"+"\n");
    before_all_tests();
    foreach (string testFn in testFns) {
        currentTestLog = "";
        timeBefore = rusage()["utime"] + rusage()["stime"];
        before_each_test();
        failingExpectsBefore = failingExpects;
        passingExpectsBefore = passingExpects;
        call_other(this_object(), testFn);
        if (failingExpects == failingExpectsBefore && passingExpects == passingExpectsBefore) {
            currentTestLog += "\n    " + ORANGE + "-" + RESET + " Warning: no expects found.";
        }

        after_each_test();
        timeAfter = rusage()["utime"] + rusage()["stime"];

        currentTestLog = "  " + UNDERLINE + BOLD + testFn + RESET + " (" + ORANGE + (timeAfter - timeBefore) + RESET + " ms):" + currentTestLog;
        write(currentTestLog+"\n");
    }


    after_all_tests();
    write("  " + passingExpects + " Pass " + (failingExpects ? failingExpects + " Fail" : "")+"\n");
    if (sizeof(testObjectUntestedFns) > 0) {
        write("  " + UNDERLINE + BOLD + "Untested Functions" + RESET + "\n");
        foreach (string fn in testObjectUntestedFns) {
            write("    " + RED + "?" + RESET + " " + fn + "\n");
        }
    }
    evaluate(done, sizeof(testFns), passingExpects, failingExpects, sizeof(testObjectFns), sizeof(testObjectUntestedFns));
}

// -----------------------------------------------------------------------------

protected object clone_object (string name) {
    string *fns;
    object ob = efun::clone_object(name);
    fns = filter_array(functions(ob, 2), (: function_exists($1, $2) :), ob) - (name == M_TEST ? ({}) : test_ignore());
    foreach(string fn in fns) {
        if (member_array(fn, testObjectFns) == -1) {
            testObjectFns += ({ fn });
            testObjectUntestedFns += ({ fn });
        }
    }
    return ob;
}

private string format_string_difference (string actual, string expect) {
    int n, al, el;
    string result;

    actual = replace_string(replace_string(actual, "\n", "\\n"), "\e", "\\e");
    expect = replace_string(replace_string(expect, "\n", "\\n"), "\e", "\\e");
    al = strlen(actual);
    el = strlen(expect);
    n = string_compare_same_until(actual, expect);
    result = "'" + (n > 0 ? GREEN + actual[0..(n-1)] : "") + ORANGE + actual[n..] + RED + expect[n..] + RESET + "'";
    return result;
}

private string format_array_differences (mixed *actual, mixed *expect) {
    int l;
    string result = "", a, e;
    l = sizeof(actual) < sizeof(expect) ? sizeof(expect) : sizeof(actual);
    for (int i = 0; i < l; i ++) {
        if (i < sizeof(actual)) {
            if (arrayp(actual[i])) a = implode(actual[i], ",");
            else a = actual[i];
        } else a = "";
        if (i < sizeof(expect)) {
            if (arrayp(expect[i])) e = implode(expect[i], ",");
            else e = expect[i];
        } else e = "";

        result += "\n        " + pad_left(""+i, 2) + ". " + format_string_difference(a, e);
    }
    return result;
}

// message should start with the function being tested
private void validate_expect (mixed value1, mixed value2, string message) {
    foreach (string fn in testObjectUntestedFns) {
        if (strsrch(message, fn+" ") > -1) {
            testObjectUntestedFns -= ({ fn });
        }
    }

    if (!currentTestPassed) {
        message = stringp(message) ? message : "An expect has failed.";
        if (failingExpects == -1) { // expected this error
            passingExpects ++;
            currentTestLog += "\n    " + GREEN + "+" + RESET + " " + RED + "x" + RESET + " " + message;
        } else {
            currentTestLog += "\n    " + RED + "x" + RESET + " " + message;
        }
        failingExpects ++;
    } else {
        passingExpects ++;
        message = stringp(message) ? message : "An expect passed.";
        currentTestLog += "\n    " + GREEN + "+" + RESET + " " + message;
    }

    if (!currentTestPassed) { // @TODO || displayExpects) {
        currentTestLog += " ->";
        if (arrayp(value1) && arrayp(value2)) {
            currentTestLog += format_array_differences(value1, value2);
        } else if (currentTestRegex) {
            currentTestLog += " " + replace_string(value1, value2, GREEN + value2 + RESET);
        } else {
            currentTestLog += " " + format_string_difference(value1, value2);
        }
    }
}

varargs void expect_true (mixed value, string message) {
    currentTestPassed = !!value;
    validate_expect((currentTestPassed ? "true" : "false"), "true", message);
}
varargs void expect_false (mixed value, string message) {
    currentTestPassed = !value;
    validate_expect((currentTestPassed ? "false" : "true"), "false", message);
}
varargs void expect_arrays_array_equal (mixed *left, mixed *right, string message) {
    mixed *rightArr = ({});
    for (int i = 0; i < sizeof(left); i ++) {
        if (sizeof(left[i]) == sizeof(right)) {
            currentTestPassed = 1;
            for (int j = 0; j < sizeof(left[i]); j ++) {
                if (left[i][j] != right[j]) {
                    currentTestPassed = 0;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < sizeof(left); i ++) {
        rightArr += ({ right });
    }
    validate_expect(left, rightArr, message);
}
varargs void expect_array_strings_equal (string *left, string right, string message) {
    string *rightArr = ({});
    currentTestPassed = 0;
    for (int i = 0; i < sizeof(left); i ++) {
        if (left[i] != right) {
            currentTestPassed = 0;
            break;
        }
        currentTestPassed = 1;
    }
    for (int i = 0; i < sizeof(left); i ++) {
        rightArr += ({ right });
    }
    validate_expect(left, rightArr, message);

}
varargs void expect_arrays_equal (mixed *left, mixed *right, string message) {
    currentTestPassed = 0;
    if (sizeof(left) == sizeof(right)) {
        currentTestPassed = 1;
        for (int i = 0; i < sizeof(left); i ++) {
            if (identify(left[i]) != identify(right[i])) {
                currentTestPassed = 0;
                break;
            }
        }
    }
    left = map_array(left, (:identify:));
    right = map_array(right, (:identify:));
    validate_expect(left, right, message);
}
varargs void expect_strings_equal (string left, string right, string message) {
    currentTestPassed = (left == right);
    validate_expect(left, right, message);
}
varargs void expect_strings_regexp (string left, string right, string message) {
    currentTestRegex = 1;
    currentTestPassed = strlen(left) > 0 && strlen(right) > 0 && regexp(left, right);
    validate_expect(left, right, message);
    currentTestRegex = 0;
}
varargs void expect_integers_equal (int left, int right, string message) {
    currentTestPassed = (left == right);
    validate_expect(""+left, ""+right, message);
}
void expect_function (string fn, object testOb) {
    currentTestPassed = !!function_exists(fn, testOb);
    if (currentTestPassed) {
        testObjectUntestedFns -= ({ fn });
    } else {
        validate_expect ("false", "true", fn +" does not exist");
    }
}