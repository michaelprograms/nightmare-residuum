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

int query_skip_coverage() {
    return 1;
}

void before_all_tests () {
    nBeforeAll ++;
}
// void after_all_tests () {

// }
void before_each_test () {
    nBeforeEach ++;
}
void after_each_test () {
    nAfterEach ++;
}
string *test_order () {
    nTestOrder ++;
    return testOrder;
}

void test_expects_passing () {
    expect("assert_equal should pass", (: ({
        assert_equal(1, 1),
        assert_equal(1.0, 1.0),
        assert_equal("abc", "abc"),
        assert_equal(({ 1, 2, 3 }), ({ 1, 2, 3 })),
        assert_equal(([ 1: "a", ]), ([ 1: "a", ])),
    }) :));

    expect("assert_regex should pass", (: ({
        assert_regex("bat", "b[a-z]t"),
        assert_regex("cat", "c[a-z]t"),
        assert_regex("/std/item/corpse#5135", "^/std/item/corpse#[0-9]+$"),
    }) :));

    expect("assert_catch should pass", (: ({
        assert_catch((: error("Test catch") :), "*Test catch\n"),
        assert_catch((: error("Test catch 2") :), "*Test catch 2\n"),
        assert_catch((: error("Different error") :), "*Different error\n"),
    }) :));
}

void test_expects_failing () {
    expect_next_failure();
    expect("expect without asserts should fail", (: ({
        // no asserts should fail
    }) :));

    expect_next_failure();
    expect("assert_equal should fail", (: ({
        assert_equal(0, 1),
        assert_equal(0.0, 1.0),
        assert_equal("xyz", "abc"),
        assert_equal(({ -1, -2, -3 }), ({ 1, 2, 3 })),
        assert_equal(([ "a": 1, ]), ([ 1: "a", ])),
    }) :));

    expect_next_failure();
    expect("assert_regex should fail", (: ({
        assert_regex("bar", "b[a-z]t"),
        assert_regex("car", "c[a-z]t"),
        assert_regex("/std/item/coins#3155", "/std/item/corpse#[0-9]+"),
    }) :));

    expect_next_failure();
    expect("assert_catch should fail", (: ({
        assert_catch((: sprintf("%s", "No error") :), "*Test Error\n"),
        assert_catch((: sprintf("%s", "Not an error") :), "*Test Error\n"),
        assert_catch((: sprintf("%s", "Success") :), "*Test Error\n"),
    }) :));
}

void test_async_test (function done) {
    call_out_walltime(function (function done) {
        expect("async function completes", (: ({
            assert_equal(1, 1),
        }) :));
        evaluate(done);
    }, 0.01, done);
}

void test_lifecycle_events () {
    expect("lifecycle events execute in order", (: ({
        assert_equal(nBeforeAll, 1), // before_all_tests
        assert_equal(nBeforeEach, sizeof(testOrder)), // before_each_test

        assert_equal(nAfterEach, sizeof(testOrder) - 1), // after_each_test not called for this test

        assert_equal(nTestOrder, 1), // test_order
    }) :));

    expect("query_expect_catch is enabled during assert 'catch'", (: ({
        assert_equal(query_expect_catch(), 0),
        assert_catch((: query_expect_catch() && error("Catch") :), "*Catch\n"),
        assert_equal(query_expect_catch(), 0),
    }) :));
}
