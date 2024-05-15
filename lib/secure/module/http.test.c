inherit M_TEST;

void test_config () {
    expect("port is queryable and settable", (: ({
        assert_equal(testOb->query_port(), UNDEFINED),

        testOb->set_port(123),
        assert_equal(testOb->query_port(), 123),
    }) :));

    expect("url patterns are queryable and settable", (: ({
        assert_equal(testOb->query_url_patterns(), ({ })),

        testOb->add_url_pattern("^/app", "app"),
        assert_equal(testOb->query_url_patterns(), ({ ({ "^/app", "app" }) })),
    }) :));
}