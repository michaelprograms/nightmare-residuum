#include "/secure/include/ldmud/functionlist.h"

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

nosave private int currentTestPassed = 0;
nosave private int failingExpects = 0, passingExpects = 0;
nosave private int failingAsserts = 0, passingAsserts = 0;
nosave private int expectCatch = 0;
nosave private string currentTestMsg, currentTestLog, currentFailLog, totalFailLog;
nosave private mixed *leftResults, *rightResults;

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
        "__INIT", // @LDMUD
    });
}

// -----------------------------------------------------------------------------

private string *testObjectFns = ({ }), *testObjectUntestedFns = ({ });

public void execute_test (closure done) {
    string *testFns, *otherTestFns;
    int timeBefore, timeAfter;
    int failingExpectsBefore, passingExpectsBefore;

    if (sizeof(testFns = test_order()) == 0) {
        testFns = functionlist(this_object(), RETURN_FUNCTION_NAME | NAME_INHERITED | NAME_HIDDEN) - test_ignore();
    } else if (sizeof(testFns) != sizeof(functionlist(this_object(), RETURN_FUNCTION_NAME | NAME_INHERITED | NAME_HIDDEN))) {
        otherTestFns = functionlist(this_object(), RETURN_FUNCTION_NAME | NAME_INHERITED | NAME_HIDDEN) - test_ignore() - testFns;
        testFns += otherTestFns;
    }

    failingExpects = 0;
    passingExpects = 0;
    totalFailLog = "";

    write("\nEvaluating " + CYAN + UNDERLINE + object_name(this_object()) + RESET + "\n");

    before_all_tests();
    foreach (string testFn in testFns) {
        if (!function_exists(testFn, this_object())) {
            write("    " + RED + "x" + RESET + " Function "+testFn+" not found.\n");
            continue;
        }

        currentTestLog = "";
        currentFailLog = "";
        timeBefore = utime()[0] + utime()[1];

        before_each_test();
        failingExpectsBefore = failingExpects;
        passingExpectsBefore = passingExpects;
        call_other(this_object(), testFn);
        if (failingExpects == failingExpectsBefore && passingExpects == passingExpectsBefore) {
            currentTestLog += "\n    " + ORANGE + "-" + RESET + " Warning: no expects found.";
        }
        after_each_test();
        timeAfter = utime()[0] + utime()[1];

        currentTestLog = "  " + UNDERLINE + BOLD + testFn + RESET + " (" + ORANGE + sprintf("%.2f", (timeAfter-timeBefore)/1000000.0) + RESET + " ms):" + currentTestLog;
        if (this_user()) {
            message("system", currentTestLog + "\n", this_user());
        } else {
            debug_message(currentTestLog);
        }
        if (sizeof(currentFailLog) > 0) {
            totalFailLog += (sizeof(totalFailLog) > 0 ? "\n" : "") + CYAN + UNDERLINE + object_name(this_object()) + RESET + ": " + UNDERLINE + BOLD + testFn + RESET + " (" + ORANGE + sprintf("%.2f", (timeAfter-timeBefore)/1000000.0) + RESET + " ms):" + currentFailLog;
        }
    }

    // Attempt to populate testObjectUntestedFns if no tests have run
    if (!sizeof(testFns) || (passingExpects + failingExpects == 0)) {
        before_each_test();
        after_each_test();
    }

    after_all_tests();
    write("  " + passingExpects + " Pass " + (failingExpects ? failingExpects + " Fail" : "") + "\n");
    if (sizeof(testObjectUntestedFns) > 0) {
        write("  " + UNDERLINE + BOLD + "Untested Functions" + RESET + "\n");
        foreach (string fn in testObjectUntestedFns) {
            write("    " + RED + "?" + RESET + " " + fn + "\n");
        }
    }

    funcall(done, ([
        "numTests": sizeof(testFns),
        "passingExpects": passingExpects,
        "failingExpects": failingExpects,
        "testedFns": sizeof(testObjectFns - testObjectUntestedFns),
        "untestedFns": sizeof(testObjectUntestedFns),
        "passingAsserts": passingAsserts,
        "failingAsserts": failingAsserts,
        "failLog": totalFailLog,
    ]));

    if (environment(this_object())) destruct(this_object());
}

// -----------------------------------------------------------------------------

protected object clone_object (string name) {
    string *fns;
    object ob = efun::clone_object(name);
    fns = filter(functionlist(ob, RETURN_FUNCTION_NAME | NAME_INHERITED | NAME_HIDDEN), (: function_exists($1, ob) :)) - test_ignore();
    foreach (string fn in fns) {
        if (member(fn, testObjectFns) == -1) {
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

    actual = regreplace(regreplace(actual, "\n", "\\n", 1), "\e", "\\e",1 );
    expect = regreplace(regreplace(expect, "\n", "\\n",1 ), "\e", "\\e",1 );
    al = sizeof(actual);
    el = sizeof(expect);
    n = string_compare_same_until(actual, expect);
    result = "'" + (n > 0 ? GREEN + actual[0..(n-1)] : "") + ORANGE + actual[n..] + RED + expect[n..] + RESET + "'";
    return result;
}

private string format_regex_difference (string actual, string regex) {
    string *results = regexp(({ actual }), "(" + regex + ")");

    if (sizeof(results) > 0) {
        return regex + " matched: " + GREEN + results[0] + RESET;
    } else {
        return format_string_difference(actual, regex);
    }
}

varargs private string format_array_differences (mixed *actual, mixed *expect) {
    int l;
    string result = "", a, e;
    l = sizeof(actual) < sizeof(expect) ? sizeof(expect) : sizeof(actual);
    for (int i = 0; i < l; i ++) {
        if (i < sizeof(actual)) {
            if (pointerp(actual[i])) a = implode(map(actual[i], (: identify($1) :)), ",");
            else a = actual[i];
        } else a = "";
        if (i < sizeof(expect)) {
            if (pointerp(expect[i])) e = implode(map(expect[i], (: identify($1) :)), ",");
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
        if (sizeof(regexp(({ message }), fn + "[ :(]"))) {
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
        if (sizeof(value1) || sizeof(value2)) {
            currentTestLog += " ->";
        }
        if (pointerp(value1) && pointerp(value2)) {
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
// Used by test.test.c to verify failing expects
protected void expect_next_failure () {
    if (program_name(this_object())[0..<3] == regreplace(M_TEST, ".c", ".test", 1) && failingExpects == 0) {
        failingExpects --;
    }
}

void expect (string message, closure fn) {

    if (!stringp(message)) raise_error("Bad argument 1 to test->expect");
    if (!closurep(fn)) raise_error("Bad argument 2 to test->expect");

    currentTestMsg = message;
    currentTestPassed = 1;
    leftResults = ({ });
    rightResults = ({ });

    passingAsserts = 0;
    catch(funcall(fn));
    if (!passingAsserts) currentTestPassed = 0;

    validate_expect(leftResults, rightResults, currentTestMsg);

    currentTestMsg = 0;
    leftResults = 0;
    rightResults = 0;
}
void assert (mixed left, string condition, mixed right) {
    mixed leftErr, rightErr, leftResult, rightResult;

    if (!stringp(currentTestMsg)) raise_error("test->assert outside of test->expect");
    if (!stringp(condition)) raise_error("Bad argument 2 to test->assert");

    if (condition == "catch") expectCatch = 1;

    if (closurep(left)) {
        if (leftErr = catch (leftResult = funcall(left))) {
            leftResult = leftErr;
            currentTestPassed = currentTestPassed && condition == "catch";
        }
    } else {
        leftResult = left;
    }
    if (closurep(right)) {
        if (rightErr = catch (rightResult = funcall(right))) {
            rightResult = rightErr;
            currentTestPassed = currentTestPassed && condition == "catch";
        }
    } else {
        rightResult = right;
    }
    if (condition == "catch") expectCatch = 0;

    if (pointerp(leftResult) || mappingp(leftResult) || (condition == "regex" && objectp(leftResult))) {
        leftResult = identify(leftResult);
    }
    if (pointerp(rightResult) || mappingp(rightResult)) {
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
            currentTestPassed = leftResult <= rightResult;
        } else if (condition == "<") {
            currentTestPassed = leftResult < rightResult;
        } else if (condition == "<=") {
            currentTestPassed = leftResult <= rightResult;
        } else if (condition == "regex") {
            currentTestPassed = !!sizeof(regexp(({ leftResult}) , rightResult));
        } else if (condition == "catch") {
            if (stringp(leftErr) && leftErr[<1] != '\n' && rightResult[<1] == '\n') rightResult = rightResult[0..<2]; // @LDMUD
            currentTestPassed = !!leftErr && leftErr == rightResult;
        } else {
            currentTestPassed = 0;
        }
    } else {
        return;
    }

    if (currentTestPassed || failingExpects == -1) {
        passingAsserts ++;
    } else {
        failingAsserts ++;
    }
}