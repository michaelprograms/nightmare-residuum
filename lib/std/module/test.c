#define ANSI(p) "\e["+(p)+"m"
#define RESET (this_user() ? "%^RESET%^" : ANSI(0))
#define BOLD (this_user() ? "%^BOLD%^" : ANSI(1))
#define UNDERLINE (this_user() ? "%^UNDERLINE%^" : ANSI(4))
#define RED (this_user() ? "%^RED%^" : ANSI(31))
#define GREEN (this_user() ? "%^GREEN%^" : ANSI(32))
#define ORANGE (this_user() ? "%^ORANGE%^" : ANSI(33))
#define CYAN (this_user() ? "%^CYAN%^" : ANSI(36))

nosave protected mixed UNDEFINED = (([])[0]); // equivalent of UNDEFINED

// -----------------------------------------------------------------------------

nosave private int currentTestPassed = 0, currentTestRegex = 0;
nosave private int failingExpects = 0, passingExpects = 0;
nosave private int expectCatch = 0;
nosave private string currentTestLog, currentFailLog, totalFailLog = "";

int query_expect_catch () {
    return expectCatch;
}

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
        "heart_beat",
    });
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

    write("\nEvaluating " + CYAN + UNDERLINE + base_name(this_object()) + RESET + "\n");
    before_all_tests();
    foreach (string testFn in testFns) {
        if (!function_exists(testFn, this_object())) {
            write("    " + RED + "x" + RESET + " Function "+testFn+" not found.\n");
            continue;
        }
        currentTestLog = "";
        currentFailLog = "";
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
        write(currentTestLog + "\n");
        if (strlen(currentFailLog) > 0) {
            totalFailLog += (sizeof(totalFailLog) > 0 ? "\n" : "") + CYAN + UNDERLINE + base_name(this_object()) + RESET + ": " + UNDERLINE + BOLD + testFn + RESET + " (" + ORANGE + (timeAfter - timeBefore) + RESET + " ms):" + currentFailLog;
        }
    }

    after_all_tests();
    write("  " + passingExpects + " Pass " + (failingExpects ? failingExpects + " Fail" : "")+"\n");
    if (sizeof(testObjectUntestedFns) > 0) {
        write("  " + UNDERLINE + BOLD + "Untested Functions" + RESET + "\n");
        foreach (string fn in testObjectUntestedFns) {
            write("    " + RED + "?" + RESET + " " + fn + "\n");
        }
    }

    evaluate(done, ([
        "numTests": sizeof(testFns),
        "numPassed": passingExpects,
        "numFailed": failingExpects,
        "fnsTested": sizeof(testObjectFns - testObjectUntestedFns),
        "fnsUntested": sizeof(testObjectUntestedFns),
        "failingExpects": totalFailLog,
    ]));

    if (environment()) destruct(this_object());
}

// -----------------------------------------------------------------------------

protected object clone_object (string name) {
    string *fns;
    object ob = efun::clone_object(name);
    fns = filter_array(functions(ob, 2), (: function_exists($1, $2) :), ob) - test_ignore();
    foreach(string fn in fns) {
        if (member_array(fn, testObjectFns) == -1) {
            testObjectFns += ({ fn });
            testObjectUntestedFns += ({ fn });
        }
    }
    return ob;
}

// -----------------------------------------------------------------------------

private string format_string_difference (string actual, string expect) {
    int n, al, el;
    string result;

    if (!stringp(actual)) {
        actual = identify(actual);
    }
    if (!stringp(expect)) {
        expect = identify(expect);
    }

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

        result += "\n        " + sprintf("%2d", i) + ". " + format_string_difference(a, e);
    }
    return result;
}

// -----------------------------------------------------------------------------

private void validate_expect (mixed value1, mixed value2, string message) {
    // message can start with the function being tested to count as tested
    foreach (string fn in testObjectUntestedFns) {
        if (regexp(message, fn + "[ :(]") > 0) {
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
            currentFailLog += "\n    " + RED + "x" + RESET + " " + message;
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

// Compare the value to true
varargs void expect_true (mixed value, string message) {
    currentTestPassed = !!value;
    validate_expect((currentTestPassed ? "true" : "false"), "true", message);
}
// Compare the value to false
varargs void expect_false (mixed value, string message) {
    currentTestPassed = !value;
    validate_expect((currentTestPassed ? "false" : "true"), "false", message);
}
// Compare each array in the array left is equal to the array right
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
        } else {
            currentTestPassed = 0;
        }
    }
    for (int i = 0; i < sizeof(left); i ++) {
        rightArr += ({ right });
    }
    validate_expect(left, rightArr, message);
}
// Compare the string array left to the string right
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
// Compare the array left to the array right
varargs void expect_arrays_equal (mixed *left, mixed *right, string message) {
    currentTestPassed = 0;
    if (sizeof(left) == sizeof(right) && sizeof(left) > 0) {
        currentTestPassed = 1;
        for (int i = 0; i < sizeof(left); i ++) {
            if (identify(left[i]) != identify(right[i])) {
                currentTestPassed = 0;
                break;
            }
        }
    }
    left = map(left, (: identify :));
    right = map(right, (: identify :));
    validate_expect(left, right, message);
}
// Compare the string left to the string right
varargs void expect_strings_equal (string left, string right, string message) {
    currentTestPassed = (left == right);
    validate_expect(left, right, message);
}
// Compare the string left to the regular expression right
varargs void expect_strings_regexp (string left, string right, string message) {
    currentTestRegex = 1;
    currentTestPassed = strlen(left) > 0 && strlen(right) > 0 && regexp(left, right);
    validate_expect(left, right, message);
    currentTestRegex = 0;
}
// Compare the integer left to the integer right
varargs void expect_integers_equal (int left, int right, string message) {
    currentTestPassed = (left == right);
    validate_expect(""+left, ""+right, message);
}
// Assert that testOb contains a public function matching fn
void expect_function (string fn, object testOb) {
    currentTestPassed = !!function_exists(fn, testOb);
    if (currentTestPassed) {
        testObjectUntestedFns -= ({ fn });
    } else {
        validate_expect ("false", "true", fn + " does not exist");
    }
}
// Compare expr evaluation for an error matching to right
varargs void expect_catch (mixed expr, string right, string message) {
    mixed err;
    expectCatch ++;
    err = catch (evaluate(expr));
    expectCatch --;
    currentTestPassed = !!err && err == right;
    if (!err) {
        err = "Not Caught";
    }
    validate_expect (err, right, message);
}
// Compare an array of expr evaluations for any errors matching right
varargs void expect_catches (mixed *expr, string right, string message) {
    mixed err;
    string *values = ({});
    expectCatch ++;
    foreach (mixed e in expr) {
        err = catch (evaluate(e));
        if (!err) {
            values += ({ "Not Caught" });
        } else {
            values += ({ err });
        }
    }
    expectCatch --;
    expect_array_strings_equal(values, right, message);
}
// Used by test.test.c to verify failing expects
protected void expect_next_failure () {
    if (base_name(this_object()) == replace_string(M_TEST, ".c", ".test") && failingExpects == 0) {
        failingExpects --;
    }
}

nosave private string currentTestMessage;
nosave private mixed *leftResults, *rightResults;
void expect (string message, function fn) {

    if (!stringp(message)) error("Bad argument 1 to test->expect");
    if (!functionp(fn)) error("Bad argument 2 to test->expect");

    currentTestMessage = message;
    currentTestPassed = 1;
    leftResults = ({ });
    rightResults = ({ });

    evaluate(fn);

    validate_expect(leftResults, rightResults, currentTestMessage);

    currentTestMessage = 0;
    leftResults = 0;
    rightResults = 0;
}
void assert (function left, string condition, mixed right) {
    mixed err, leftResult, rightResult;

    if (!stringp(currentTestMessage)) error("test->assert outside of test->expect");
    if (!functionp(left)) error("Bad argument 1 to test->assert");
    if (!stringp(condition)) error("Bad argument 2 to test->assert");
    if (undefinedp(right)) error("Bad argument 2 to test->assert");

    err = catch (leftResult = evaluate(left));
    if (err) {
        leftResult = err;
        currentTestPassed = 0;
    }
    leftResults += ({ leftResult });

    if (functionp(right)) {
        err = catch (rightResult = evaluate(right));
        if (err) {
            rightResult = err;
            currentTestPassed = 0;
        }
    } else {
        rightResult = right;
    }
    rightResults += ({ rightResult });

    if (!currentTestPassed) return;
     else if (condition == "==") {
        currentTestPassed = !undefinedp(rightResult) && leftResult == rightResult;
    } else if (condition == ">") {
        currentTestPassed = !undefinedp(rightResult) && leftResult > rightResult;
    } else if (condition == ">=") {
        currentTestPassed = !undefinedp(rightResult) && leftResult <= rightResult;
    } else if (condition == "<") {
        currentTestPassed = !undefinedp(rightResult) && leftResult < rightResult;
    } else if (condition == "<=") {
        currentTestPassed = !undefinedp(rightResult) && leftResult <= rightResult;
    }
}