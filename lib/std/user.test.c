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
    expect("receive_environ adjusts terminal", (: ({
        // initial state
        testOb->set_terminal("ip", "ipaddress1"),
        testOb->set_terminal("client", "client1"),
        testOb->set_terminal("version", "version1"),
        assert_equal(testOb->query_terminal("ip"), "ipaddress1"),
        assert_equal(testOb->query_terminal("client"), "client1"),
        assert_equal(testOb->query_terminal("version"), "version1"),

        testOb->receive_environ("IPADDRESS", "ipaddress2"),
        assert_equal(testOb->query_terminal("ip"), "ipaddress2 (ipaddress1)"),

        testOb->receive_environ("CLIENT_NAME", "client2"),
        assert_equal(testOb->query_terminal("client"), "client2"),

        testOb->receive_environ("CLIENT_VERSION", "version2"),
        assert_equal(testOb->query_terminal("version"), "version2"),
    }) :));
}