inherit M_TEST;

private int action = 0;

string *test_order () {
    return ({ "test_set_variables", "test_set_variable_hooks" });
}

void test_set_variables () {
    expect("variables handles set/query", (: ({
        assert_equal(testOb->query_variable("testkey1"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey1", 123), 123),
        assert_equal(testOb->query_variable("testkey1"), 123),

        assert_equal(testOb->unset_variable("testkey1"), 1),
        assert_equal(testOb->query_variable("testkey1"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey1", "xyz"), "xyz"),
        assert_equal(testOb->query_variable("testkey1"), "xyz"),
    }) :));

    expect("variable_if_undefined handled", (: ({
        assert_equal(testOb->query_variable("testkey2"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey2", 123, 1), 123),
        assert_equal(testOb->query_variable("testkey2"), 123),

        assert_equal(testOb->set_variable("testkey2", 987, 1), 123), // didn't set

        assert_equal(testOb->unset_variable("testkey2"), 1),
        assert_equal(testOb->query_variable("testkey2"), UNDEFINED),

        assert_equal(testOb->set_variable("testkey2", 987, 1), 987), // did set
    }) :));
}

void test_set_variable_hooks () {
    testOb->set_variable_hook("testkey", function (mixed value) {
        action ++;
        return value;
    });

    expect("set_variable_hook handles hook", (: ({
        assert_equal(action, 0),

        assert_equal(testOb->set_variable("testkey", 123), 123),
        assert_equal(action, 1),

        assert_equal(testOb->set_variable("testkey", "xyz"), "xyz"),
        assert_equal(action, 2),
    }) :));
}