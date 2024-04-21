inherit M_TEST;

void test_type () {
    expect("type is settable and queryable", (: ({
        assert_equal(testOb->query_type(), UNDEFINED),

        testOb->set_type("test"),
        assert_equal(testOb->query_type(), "test"),

        testOb->set_type("test2"),
        assert_equal(testOb->query_type(), "test2"),
    }) :));
}