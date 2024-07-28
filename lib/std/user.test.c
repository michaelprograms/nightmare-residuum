inherit M_TEST;

void test_user () {
    expect("is_living returns true", (: ({
        assert_equal(testOb->is_user(), 1),
        assert_equal(testOb->is_living(), 0),
        assert_equal(testOb->is_character(), 0),
    }) :));
}

void test_terminal () {
    expect("user terminal initializes to default", (: ({
        assert_equal(testOb->query_terminal("width"), 80),
        assert_equal(testOb->query_terminal("height"), 40),

        testOb->set_terminal("width", 76),
        assert_equal(testOb->query_terminal("width"), 76),

        testOb->window_size(60, 30),
        assert_equal(testOb->query_terminal("width"), 60),
        assert_equal(testOb->query_terminal("height"), 30),
    }) :));
}