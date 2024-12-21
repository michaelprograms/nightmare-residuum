inherit M_TEST;

/**
 * @var {"/std/user/shell"} testOb
 */

private int action = 0;

void test_variables () {
    expect("variables are queryable and settable", (: ({
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

void test_variable_hooks () {
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

void test_aliases () {
    expect("aliases are queryable, settable, and removable", (: ({
        assert_equal(testOb->query_alias_names(), ({ "n", "nw", "l", "cl", "ne", "'", "ent", "w", "sw", "u", "e", "d", "se", "s" })),
        assert_equal(testOb->query_alias("testalias1"), UNDEFINED),

        testOb->set_alias("testalias1", "test alias 1 expansion"),
        assert_equal(testOb->query_alias("testalias1"), ([ "d": ({ "" }), "n": UNDEFINED, "t": "test alias 1 expansion $*" ])),

        testOb->unset_alias("testalias1"),
        assert_equal(testOb->query_alias("testalias1"), UNDEFINED),

        testOb->set_alias("testalias1", "xyz"),
        assert_equal(testOb->query_alias("testalias1"), ([ "d": ({ "" }), "n": UNDEFINED, "t": "xyz $*" ])),
    }) :));
}