inherit M_TEST;

void test_level () {
    expect("level settable and queryable", (: ({
        assert_equal(testOb->query_level(), 1),

        testOb->set_level(123),
        assert_equal(testOb->query_level(), 123),

        testOb->set_level(0),
        assert_equal(testOb->query_level(), 0),

        testOb->set_level(-123),
        assert_equal(testOb->query_level(), 123),
    }) :));
}