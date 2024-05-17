#define ANSI(p)     "\e["+(p)+"m"
#define RESET       ANSI(0)
#define BOLD        ANSI(1)
#define UNDERLINE   ANSI(4)
#define BLACK       ANSI(30)
#define RED         ANSI(31)
#define GREEN       ANSI(32)
#define ORANGE      ANSI(33)
#define CYAN        ANSI(36)
#define B_RED       ANSI(41)
#define B_GREEN     ANSI(42)
#define B_ORANGE    ANSI(43)

#define TEST_IGNORE_DEFAULTS ({ "before_all_tests", "before_each_test", "after_each_test", "after_all_tests", "test_order", "test_ignore", "execute_test", "create", "init", "reset", "heart_beat", })

nosave protected mixed UNDEFINED = (([ ])[0]); // equivalent of UNDEFINED

/* ----- function prototypes ----- */

private void process_test ();

/* ----- variable definitions ----- */

nosave object testOb;
nosave string testFile;

nosave private int currentTestNum = 0, currentTestPassed = 0;
nosave private int failingExpects = 0, passingExpects = 0;
nosave private int failingAsserts = 0, passingAsserts = 0, totalPassingAsserts = 0;
nosave private int expectCatch = 0;
nosave private string currentTestFn, currentTestMsg, currentTestLog, currentFailLog, totalFailLog;
nosave private mixed *leftResults, *rightResults;
nosave private string *testFunctions;
int timeBefore, timeAfter, timeTotalBefore, timeTotalAfter, failingExpectsBefore, passingExpectsBefore;
private function doneTestFn;

/* ----- override functions ----- */

void before_all_tests () {

}
void before_each_test () {

}
void after_each_test () {

}
void after_all_tests () {

}
string *test_order () {
    return 0;
}
string *test_ignore () {
    return ({ });
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

private int query_async_test_function (string fn) {
    mixed *fns;

    fns = filter(functions(this_object(), 3), (: $1[0] == $(fn) :));
    return sizeof(fns) == 1 && fns[0][1] == 1 && fns[0][3] == "function";
}

/* -----  ----- */

private void finish_test () {
    if (!this_object()->query_skip_coverage() && regexp(testFile, "\\.coverage\\.")) {
        rm(testFile);
    }
    after_all_tests();
    timeTotalAfter = time_ns();

    if (D_TEST->query_option("brief")) {
        string status = "";
        if (totalPassingAsserts > 0 && !failingAsserts) {
            status += "\e[32m\u2713 \e[0m";
        } else if (!totalPassingAsserts) {
            status += "\e[33m\u203C \e[0m";
        } else {
            status += "\e[31m\u2715 \e[0m";
        }
        write(status + " Evaluated " + CYAN + UNDERLINE + base_name() + RESET + " (" + ORANGE + sprintf("%.2f", (timeTotalAfter-timeTotalBefore)/1000000.0) + " ms" + RESET ")\n");
    }

    evaluate(doneTestFn, ([
        "numTests": sizeof(testFunctions),
        "passingExpects": passingExpects,
        "failingExpects": failingExpects,
        "testedFns": sizeof(D_TEST->query_hit_functions()),
        "untestedFns": sizeof(D_TEST->query_unhit_functions()),
        "passingAsserts": totalPassingAsserts,
        "failingAsserts": failingAsserts,
        "failLog": totalFailLog,
        "hitLines": sizeof(D_TEST->query_hit_lines()),
        "unhitLines": sizeof(D_TEST->query_unhit_lines()),
    ]));


    destruct();
}

public int execute_test (function done) {
    timeTotalBefore = time_ns();
    doneTestFn = done;

    // reset test metrics
    failingExpects = 0;
    passingExpects = 0;
    totalFailLog = "";

    // @TODO simplify this
    testFunctions = test_order();
    if (!sizeof(testFunctions)) {
        testFunctions = functions(this_object(), 2) - TEST_IGNORE_DEFAULTS - test_ignore();
    } else if (sizeof(testFunctions) != sizeof(functions(this_object(), 2))) {
        // grab any tests that were not included in test_order and test_ignore
        testFunctions += (functions(this_object(), 2) - TEST_IGNORE_DEFAULTS - test_ignore() - testFunctions);
    }
    testFunctions = filter(testFunctions, (: regexp($1, "test_") :));

    if (!D_TEST->query_option("brief")) {
        write("\nEvaluating " + CYAN + UNDERLINE + base_name() + RESET + "\n");
    }

    if (this_object()->query_skip_coverage()) {
        testFile = replace_string(base_name(), ".test", ".c");
    } else {
        testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
    }
    before_all_tests();

    if (currentTestNum < sizeof(testFunctions)) {
        process_test();
    } else {
        finish_test();
    }
}

private void done_current_test () {
    string status = "";

    timeAfter = time_ns();
    if (objectp(testOb)) {
        testOb->handle_remove();
        if (testOb) {
            destruct(testOb);
        }
    }
    after_each_test();

    if (totalPassingAsserts > 0 && !failingAsserts) {
        status += "\e[32m\u2713 \e[0m";
    } else {
        status += "\e[31m\u2715 \e[0m";
    }
    if (!D_TEST->query_option("brief")) {
        currentTestLog = "  " + status + "Testing " + BOLD + UNDERLINE + currentTestFn + RESET + " (" + ORANGE + sprintf("%.2f", (timeAfter-timeBefore)/1000000.0) + " ms" + RESET + ")" + currentTestLog;
        write(currentTestLog + "\n");
    }

    if (passingExpects + failingExpects == 0) {
        write("  No Tests "+identify(this_object())+"\n");
    } else if (strlen(currentFailLog) > 0) {
        totalFailLog += (sizeof(totalFailLog) > 0 ? "\n" : "") + CYAN + UNDERLINE + base_name() + RESET + ": " + UNDERLINE + BOLD + currentTestFn + RESET + " (" + ORANGE + sprintf("%.2f", (timeAfter-timeBefore)/1000000.0) + " ms" + RESET + "):" + currentFailLog;
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

        testOb = new(testFile);
        before_each_test();
        failingExpectsBefore = failingExpects;
        passingExpectsBefore = passingExpects;

        timeBefore = time_ns();
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

    if (!stringp(actual)) {
        actual = identify(actual);
    }
    if (!stringp(expect)) {
        expect = identify(expect);
    }

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
            if (arrayp(actual[i])) {
                a = implode(map(actual[i], (: identify($1) :)), ",");
            } else {
                a = actual[i];
            }
        } else {
            a = "";
        }
        if (i < sizeof(expect)) {
            if (arrayp(expect[i])) {
                e = implode(map(expect[i], (: identify($1) :)), ",");
            } else {
                e = expect[i];
            }
        } else {
            e = "";
        }

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

/* ----- expect and assert ----- */

private void validate_expect (mixed value1, mixed value2, string message) {
    if (!currentTestPassed) {
        message = stringp(message) ? message : "An expect has failed.";
        if (failingExpects == -1) { // expected this error
            passingExpects ++;
            if (!D_TEST->query_option("brief")) {
                currentTestLog += "\n" + GREEN + "    +" + RESET + RED + " x" + RESET + " " + message;
            }
        } else if (!D_TEST->query_option("brief")) {
            currentTestLog += "\n" + RED + "    x" + RESET + " " + message;
            currentFailLog += "\n" + RED + "    x" + RESET + " " + message;
        }
        failingExpects ++;
    } else {
        passingExpects ++;
        if (!D_TEST->query_option("brief")) {
            message = stringp(message) ? message : "An expect passed.";
            currentTestLog += "\n" + GREEN + "    +" + RESET + " " + message;
        }
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

void expect (string message, function fn) {
    if (!stringp(message)) {
        error("Bad argument 1 to test->expect");
    }
    if (!functionp(fn)) {
        error("Bad argument 2 to test->expect");
    }

    currentTestMsg = message;
    currentTestPassed = 1;
    leftResults = ({ });
    rightResults = ({ });

    passingAsserts = 0;
    catch (evaluate(fn));
    if (!passingAsserts) {
        currentTestPassed = 0;
    }

    validate_expect(leftResults, rightResults, currentTestMsg);

    currentTestMsg = 0;
    leftResults = 0;
    rightResults = 0;
}
void assert_equal (mixed left, mixed right) {
    if (!stringp(currentTestMsg)) {
        error("test->assert_equal outside of test->expect");
    }

    if (arrayp(left) || mapp(left) || objectp(left)) {
        left = identify(left);
    }
    if (arrayp(right) || mapp(right) || objectp(right)) {
        right = identify(right);
    }

    if (currentTestPassed) {
        if (typeof(left) == "float" && typeof(right) == "float") {
            left = to_float("" + left);
            right = to_float("" + right);
        }
        currentTestPassed = left == right;
    }

    leftResults += ({ left });
    rightResults += ({ right });

    if (currentTestPassed || failingExpects == -1) {
        passingAsserts ++;
        totalPassingAsserts ++;
    } else {
        failingAsserts ++;
    }
}
void assert_regex (mixed left, string right) {
    if (!stringp(currentTestMsg)) {
        error("test->assert_regex outside of test->expect");
    }

    if (arrayp(left) || mapp(left) || objectp(left)) {
        left = identify(left);
    }

    if (currentTestPassed) {
        currentTestPassed = regexp(left, right) > 0;
    }

    leftResults += ({ left });
    rightResults += ({ right });

    if (currentTestPassed || failingExpects == -1) {
        passingAsserts ++;
        totalPassingAsserts ++;
    } else {
        failingAsserts ++;
    }
}
void assert_catch (function left, string right) {
    mixed leftResult, leftError;

    if (!stringp(currentTestMsg)) {
        error("test->assert_catch outside of test->expect");
    }

    expectCatch = 1;
    if (leftError = catch(leftResult = evaluate(left))) {
        leftResult = leftError;
    }
    expectCatch = 0;

    if (arrayp(leftResult) || mapp(leftResult)) {
        leftResult = identify(leftResult);
    }
    leftResults += ({ leftResult });
    rightResults += ({ right });

    if (currentTestPassed) {
        currentTestPassed = !!leftError && leftError == right;
    }

    if (currentTestPassed || failingExpects == -1) {
        passingAsserts ++;
        totalPassingAsserts ++;
    } else {
        failingAsserts ++;
    }
}
