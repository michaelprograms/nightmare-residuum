inherit M_TEST;

private int action = 0;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/shell/variable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_order () {
    return ({ "test_set_variables", "test_set_variable_hooks" });
}

void test_set_variables () {
    expect_function("query_variable", testOb);
    expect_function("set_variable", testOb);
    expect_function("unset_variable", testOb);

    expect("variables handles set/query", (: ({
        assert(testOb->query_variable("testkey1"), "==", UNDEFINED),

        assert(testOb->set_variable("testkey1", 123), "==", 123),
        assert(testOb->query_variable("testkey1"), "==", 123),

        assert(testOb->unset_variable("testkey1"), "==", 1),
        assert(testOb->query_variable("testkey1"), "==", UNDEFINED),

        assert(testOb->set_variable("testkey1", "xyz"), "==", "xyz"),
        assert(testOb->query_variable("testkey1"), "==", "xyz"),
    }) :));

    expect("variable_if_undefined handled", (: ({
        assert(testOb->query_variable("testkey2"), "==", UNDEFINED),

        assert(testOb->set_variable("testkey2", 123, 1), "==", 123),
        assert(testOb->query_variable("testkey2"), "==", 123),

        assert(testOb->set_variable("testkey2", 987, 1), "==", 123), // didn't set

        assert(testOb->unset_variable("testkey2"), "==", 1),
        assert(testOb->query_variable("testkey2"), "==", UNDEFINED),

        assert(testOb->set_variable("testkey2", 987, 1), "==", 987), // did set
    }) :));
}

void test_set_variable_hooks () {
    expect_function("set_variable_hook", testOb);

    testOb->set_variable_hook("testkey", function (mixed value) {
        action ++;
        return value;
    });

    expect("set_variable_hook handles hook", (: ({
        assert(action, "==", 0),

        assert(testOb->set_variable("testkey", 123), "==", 123),
        assert(action, "==", 1),

        assert(testOb->set_variable("testkey", "xyz"), "==", "xyz"),
        assert(action, "==", 2),
    }) :));
}