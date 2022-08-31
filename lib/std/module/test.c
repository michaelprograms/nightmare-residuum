#define ANSI(p) "\e["+(p)+"m"
#define RESET (this_user() ? "%^RESET%^" : ANSI(0))
#define BOLD (this_user() ? "%^BOLD%^" : ANSI(1))
#define UNDERLINE (this_user() ? "%^UNDERLINE%^" : ANSI(4))
#define BLACK (this_user() ? "%^BLACK%^" : ANSI(30))
#define RED (this_user() ? "%^RED%^" : ANSI(31))
#define GREEN (this_user() ? "%^GREEN%^" : ANSI(32))
#define ORANGE (this_user() ? "%^ORANGE%^" : ANSI(33))
#define CYAN (this_user() ? "%^CYAN%^" : ANSI(36))
#define B_RED (this_user() ? "%^B_RED%^" : ANSI(41))
#define B_GREEN (this_user() ? "%^B_GREEN%^" : ANSI(42))
#define B_ORANGE (this_user() ? "%^B_ORANGE%^BLACK%^" : ANSI(43))

nosave protected mixed UNDEFINED = (([])[0]); // equivalent of UNDEFINED

/* -----  ----- */

private void process_test ();

/* -----  ----- */

nosave private int currentTestNum = 0, currentTestPassed = 0;
nosave private int failingExpects = 0, passingExpects = 0;
nosave private int failingAsserts = 0, passingAsserts = 0, totalPassingAsserts = 0;
nosave private int expectCatch = 0;
nosave private string currentTestFn, currentTestMsg, currentTestLog, currentFailLog, totalFailLog;
nosave private mixed *leftResults, *rightResults;
nosave private string *testFunctions, *testObjectFns = ({ }), *testObjectUntestedFns = ({ });
int timeBefore, timeAfter, failingExpectsBefore, passingExpectsBefore;
private function doneTestFn;

/* ----- overrideable functions ----- */

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

/* ----- test functions ----- */

// Used by test.test.c to verify failing expects
protected void expect_next_failure () {
    if (base_name() == replace_string(M_TEST, ".c", ".test") && failingExpects == 0) {
        failingExpects --;
    }
}
// Used by test.test.c to verify catches
int query_expect_catch () {
    return expectCatch;
}

/* ----- helper functions ----- */

protected object clone_object (string path) {
    string *fns;
    object ob;

    if (file_size(path) < 1) {
        error("Invalid path passed to test->clone_object");
    }
    ob = efun::clone_object(path);
    fns = filter_array(functions(ob, 2), (: function_exists($1, $2) :), ob) - test_ignore();
    foreach (string fn in fns) {
        if (member_array(fn, testObjectFns) == -1) {
            testObjectFns += ({ fn });
            testObjectUntestedFns += ({ fn });
        }
    }
    return ob;
}

private int query_async_test_function (string fn) {
    mixed *fns;

    fns = filter(functions(this_object(), 3), (: $1[0] == $(fn) :));
    return sizeof(fns) == 1 && fns[0][1] == 1 && fns[0][3] == "function";
}

private string *query_test_functions () {
    string *fns;

    fns = test_order();

    if (!sizeof(fns)) {
        fns = functions(this_object(), 2) - test_ignore();
    } else if (sizeof(fns) != sizeof(functions(this_object(), 2))) {
        // grab any tests that were not included in test_order and test_ignore
        string *otherTestFns = functions(this_object(), 2) - test_ignore() - fns;
        fns += otherTestFns;
    }

    fns = filter(fns, (: regexp($1, "test_") :));

    return fns;
}

/* -----  ----- */

private void finish_test () {
    // Attempt to populate testObjectUntestedFns if no tests have run
    if (!sizeof(testFunctions) || (passingExpects + failingExpects == 0)) {
        before_each_test();
        after_each_test();
    }

    after_all_tests();
    write("  " + passingExpects + " Pass " + (failingExpects ? failingExpects + " Fail" : "") + "\n");
    if (sizeof(testObjectUntestedFns) > 0) {
        write("\n  Found " + BOLD + UNDERLINE + "Untested Functions" + RESET + "\n");
        foreach (string fn in testObjectUntestedFns) {
            write(RED + "    ?" + RESET + " " + fn + "\n");
        }
    }

    evaluate(doneTestFn, ([
        "numTests": sizeof(testFunctions),
        "passingExpects": passingExpects,
        "failingExpects": failingExpects,
        "testedFns": sizeof(testObjectFns - testObjectUntestedFns),
        "untestedFns": sizeof(testObjectUntestedFns),
        "passingAsserts": totalPassingAsserts,
        "failingAsserts": failingAsserts,
        "failLog": totalFailLog,
    ]));

    destruct();
}

public int execute_test (function done) {

    doneTestFn = done;

    // reset test metrics
    failingExpects = 0;
    passingExpects = 0;
    totalFailLog = "";

    testFunctions = query_test_functions();

    write("\nEvaluating " + CYAN + UNDERLINE + base_name() + RESET + "\n");

    before_all_tests();

    if (currentTestNum < sizeof(testFunctions)) {
        process_test();
    } else {
        finish_test();
    }
}

private void done_current_test () {
    timeAfter = perf_counter_ns();
    if (failingExpects == failingExpectsBefore && passingExpects == passingExpectsBefore) {
        currentTestLog += "\n" + ORANGE + "    -" + RESET + " Warning: no expects found.";
    }
    after_each_test();

    currentTestLog = "  Testing " + BOLD + UNDERLINE + currentTestFn + RESET + " (" + ORANGE + sprintf("%.2f", (timeAfter-timeBefore)/1000000.0) + RESET + " ms):" + currentTestLog;
    if (this_user()) {
        message("system", currentTestLog + "\n", this_user());
    } else {
        debug_message(currentTestLog);
    }
    if (strlen(currentFailLog) > 0) {
        totalFailLog += (sizeof(totalFailLog) > 0 ? "\n" : "") + CYAN + UNDERLINE + base_name() + RESET + ": " + UNDERLINE + BOLD + currentTestFn + RESET + " (" + ORANGE + sprintf("%.2f", (timeAfter-timeBefore)/1000000.0) + RESET + " ms):" + currentFailLog;
    }

    currentTestNum ++;
    if (currentTestNum < sizeof(testFunctions)) {
        process_test();
    } else {
        finish_test();
    }
}

private void process_test () {
    // no tests remaining
    if (currentTestNum >= sizeof(testFunctions)) {
        done_current_test();
        return;
    }

    currentTestFn = testFunctions[currentTestNum];

    // test function doesn't exist
    if (!function_exists(currentTestFn, this_object())) {
        currentFailLog = "\n" + RED + "    x" + RESET + " Function " + currentTestFn + " not found.";
        done_current_test();
    } else if (regexp(currentTestFn, "test_")) {
        currentTestLog = "";
        currentFailLog = "";

        before_each_test();
        failingExpectsBefore = failingExpects;
        passingExpectsBefore = passingExpects;

        timeBefore = perf_counter_ns();
        if (query_async_test_function(currentTestFn)) {
            call_other(this_object(), currentTestFn, (: done_current_test :));
        } else {
            call_other(this_object(), currentTestFn);
            done_current_test();
        }
    } else {
        currentFailLog = "\n" + RED + "    x" + RESET + " Function " + currentTestFn + " not a test.";
        done_current_test();
    }
}

/* ----- display diff helper functions ----- */

private string format_string_difference (string actual, string expect) {
    int n;
    string result;

    if (!stringp(actual)) actual = identify(actual);
    if (!stringp(expect)) expect = identify(expect);

    actual = replace_string(replace_string(replace_string(actual, "\n", "\\n"), "\e", "\\e"), "%^", "%%^%^^");
    expect = replace_string(replace_string(replace_string(expect, "\n", "\\n"), "\e", "\\e"), "%^", "%%^%^^");
    n = string_compare_same_until(actual, expect);
    result = "'" + BLACK + (n ? B_GREEN + actual[0..n-1] : "") + B_ORANGE + actual[n..] + B_RED + expect[n..] + RESET + "'";
    return result;
}

private string format_regex_difference (string actual, string regex) {
    string *results = pcre_extract(actual, "(" + regex + ")");
    if (sizeof(results) > 0) {
        return regex + " matched: " + GREEN + results[0] + RESET;
    } else {
        return format_string_difference(actual, regex);
    }
}

varargs private string format_array_differences (mixed *actual, mixed *expect) {
    string result = "", a, e;
    int l = max(({ sizeof(actual), sizeof(expect) }));

    for (int i = 0; i < l; i ++) {
        if (i < sizeof(actual)) {
            if (arrayp(actual[i])) a = implode(map(actual[i], (: identify($1) :)), ",");
            else a = actual[i];
        } else a = "";
        if (i < sizeof(expect)) {
            if (arrayp(expect[i])) e = implode(map(expect[i], (: identify($1) :)), ",");
            else e = expect[i];
        } else e = "";

        if (stringp(e) && e[0..0] == "/" && e[<1..<1] == "/") {
            e = e[1..<2];
            result += "\n      " + sprintf("%2d", i) + ". ";
            result += format_regex_difference(a, e);

        } else {
            result += "\n      " + sprintf("%2d", i) + ". " + format_string_difference(a, e);
        }
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
            currentTestLog += "\n" + GREEN + "    +" + RESET + RED + " x" + RESET + " " + message;
        } else {
            currentTestLog += "\n" + RED + "    x" + RESET + " " + message;
            currentFailLog += "\n" + RED + "    x" + RESET + " " + message;
        }
        failingExpects ++;
    } else {
        passingExpects ++;
        message = stringp(message) ? message : "An expect passed.";
        currentTestLog += "\n" + GREEN + "    +" + RESET + " " + message;
    }

    if (!currentTestPassed) { // @TODO || displayExpects) {
        if (sizeof(value1) || sizeof(value2)) {
            currentTestLog += " ->";
        }
        if (arrayp(value1) && arrayp(value2)) {
            currentTestLog += format_array_differences(value1, value2);
        } else {
            currentTestLog += " " + format_string_difference(value1, value2);
        }
    }
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

void expect (string message, function fn) {
    if (!stringp(message)) error("Bad argument 1 to test->expect");
    if (!functionp(fn)) error("Bad argument 2 to test->expect");

    currentTestMsg = message;
    currentTestPassed = 1;
    leftResults = ({ });
    rightResults = ({ });

    totalPassingAsserts += passingAsserts;
    passingAsserts = 0;
    catch (evaluate(fn));
    if (!passingAsserts) currentTestPassed = 0;

    validate_expect(leftResults, rightResults, currentTestMsg);

    currentTestMsg = 0;
    leftResults = 0;
    rightResults = 0;
}
void assert (mixed left, string condition, mixed right) {
    mixed leftErr, rightErr, leftResult, rightResult;

    if (!stringp(currentTestMsg)) error("test->assert outside of test->expect");
    if (!stringp(condition)) error("Bad argument 2 to test->assert");

    if (condition == "catch") expectCatch = 1;

    if (functionp(left)) {
        if (leftErr = catch (leftResult = evaluate(left))) {
            leftResult = leftErr;
            currentTestPassed = currentTestPassed && condition == "catch";
        }
    } else {
        leftResult = left;
    }
    if (functionp(right)) {
        if (rightErr = catch (rightResult = evaluate(right))) {
            rightResult = rightErr;
            currentTestPassed = currentTestPassed && condition == "catch";
        }
    } else {
        rightResult = right;
    }
    if (condition == "catch") expectCatch = 0;

    if (arrayp(leftResult) || mapp(leftResult) || (condition == "regex" && objectp(leftResult))) {
        leftResult = identify(leftResult);
    }
    if (arrayp(rightResult) || mapp(rightResult)) {
        rightResult = identify(rightResult);
    }

    leftResults += ({ leftResult });
    rightResults += ({
        (condition == "regex" ? "/" + rightResult + "/" : rightResult)
    });

    if (currentTestPassed) {
        if (condition == "==") {
            currentTestPassed = leftResult == rightResult;
        } else if (condition == "!=") {
            currentTestPassed = leftResult != rightResult;
        } else if (condition == ">") {
            currentTestPassed = leftResult > rightResult;
        } else if (condition == ">=") {
            currentTestPassed = leftResult >= rightResult;
        } else if (condition == "<") {
            currentTestPassed = leftResult < rightResult;
        } else if (condition == "<=") {
            currentTestPassed = leftResult <= rightResult;
        } else if (condition == "regex") {
            currentTestPassed = regexp(leftResult, rightResult) > 0;
        } else if (condition == "catch") {
            currentTestPassed = !!leftErr && leftErr == rightResult;
        } else {
            currentTestPassed = 0;
        }
    }

    if (currentTestPassed || failingExpects == -1) {
        passingAsserts ++;
    } else {
        failingAsserts ++;
    }
}