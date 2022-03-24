inherit M_TEST;

private int nBeforeAll = 0;
private int nBeforeEach = 0;
private int nAfterEach = 0;
private int nTestOrder = 0;
private string *testOrder = ({
    "test_expects_passing",
    "test_expects_failing",
    "test_lifecycle_events",
});

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/module/test.c");
    nBeforeAll ++;
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
void before_each_test () { nBeforeEach ++; }
void after_each_test () { nAfterEach ++; }
string *test_order () {
    nTestOrder ++;
    return testOrder;
}
string *test_ignore () { return ::test_ignore() + ({ "test_should_be_ignored" }); }

void test_expects_passing () {
    string *arr1 = ({ "abc", "123", "!@#", });

    expect_function("expect_function", testOb);
    expect_function("expect", testOb);
    expect_function("assert", testOb);

    expect_true(1, "expect_true(1) should pass");
    expect_false(0, "expect_false(0) should pass");

    expect_strings_equal("abc", "abc", "expect_strings_equal(str1, str1, msg) should pass");

    expect_strings_regexp("bat cat rat", "cat", "expect_strings_regexp(str, regexp, msg) should pass");

    expect_integers_equal(12345, 12345, "expect_integers_equal(int1, int1, msg) should pass");

    expect_arrays_equal(arr1, arr1, "expect_arrays_equal(arr1, arr1, msg) should pass");

    expect_array_strings_equal(({
        "abc",
        "abc",
        "abc",
    }), "abc", "expect_array_strings_equal(arr, str, msg) should pass");

    expect_arrays_array_equal(({
        arr1,
        arr1,
        arr1,
    }), arr1, "expect_arrays_array_equal(arrOfArrs, *arr, msg) should pass");

    expect("expect > assert 'catch' catches errors", (: ({
        assert((: error("Test catch") :), "catch", "*Test catch\n"),
        assert((: error("Test catch 2") :), "catch", "*Test catch 2\n"),
        assert((: error("Different error") :), "catch", "*Different error\n"),
    }) :));
}

void test_expects_failing () {
    string *arr1 = ({ "abc", "123", "!@#", });
    string *arr2 = ({ "XYZ", "999", ",.'", });

    expect_next_failure();
    expect_true(0, "expect_true(0) should fail");

    expect_next_failure();
    expect_false(1, "expect_false(1) should fail");

    expect_next_failure();
    expect_strings_equal("abc", "123", "expect_strings_equal(str1, str2) should fail");

    expect_next_failure();
    expect_strings_regexp("bat cat rat", "fat", "expect_strings_regexp(str, regexp, msg) should fail");

    expect_next_failure();
    expect_integers_equal(12345, 67890, "expect_integers_equal(int1, int2, msg) should fail");

    expect_next_failure();
    expect_arrays_equal(arr1, arr2, "expect_arrays_equal(*arr1, *arr2, msg) should fail");
    expect_next_failure();
    expect_arrays_equal(arr1, arr1[0..<2], "expect_arrays_equal(*arr1, *arr1[0..<2], msg) should fail");
    expect_next_failure();
    expect_arrays_equal(arr1[1..], arr1, "expect_arrays_equal(*arr1, *arr1[0..<2], msg) should fail");

    expect_next_failure();
    expect_array_strings_equal(({
        "abc",
        "123",
        "!@#",
    }), "abc", "expect_array_strings_equal(*arr, str, msg) should fail");

    expect_next_failure();
    expect_arrays_array_equal(({
        arr1,
        arr1,
        arr2,
    }), arr1, "expect_arrays_array_equal(*arrOfArrs, *arr, msg) should fail");

    expect_next_failure();
    expect_arrays_equal(
        ({ "zero giraffes", "zero giraffes", "zero giraffe", "zero giraffe", "zero giraffes" }),
        ({ "zero giraffes", "zero giraffe", "zero giraffes", "zeri giraffes", "zeri giraffe" }),
        "format_array_differences should fail"
    );

    expect_next_failure();
    expect_function("nonexistant_function", testOb);

    expect_next_failure();
    expect("expect > assert 'catch' should fail", (: ({
        assert((: "No error" :), "catch", "*Test Error\n"),
        assert((: "Not an error" :), "catch", "*Test Error\n"),
        assert((: "Success" :), "catch", "*Test Error\n"),
    }) :));
}

void test_lifecycle_events () {
    expect_true(nBeforeAll == 1, "before_all_tests() ran once");
    expect_true(nBeforeEach == sizeof(testOrder), "before_each_test() ran before each test");
    // the current test has not finished yet
    expect_true(nAfterEach == sizeof(testOrder) - 1, "after_each_test() ran after each test");
    expect_true(nTestOrder == 1, "test_order() called once");

    expect("query_expect_catch is enabled during expect > assert 'catch'", (: ({
        assert(query_expect_catch(), "==", 0),
        assert(function () {
            if (query_expect_catch()) {
                error("Catch");
            }
        }, "catch", "*Catch\n"),
        assert(query_expect_catch(), "==", 0),
    }) :));
}

void test_should_be_ignored () {
    expect_true(0, "this test should be ignored");
}