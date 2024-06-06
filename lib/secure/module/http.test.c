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

    expect("allowing static pages is queryable and settable", (: ({
        assert_equal(testOb->query_allow_static_pages(), 0),

        testOb->allow_static_pages(1),
        assert_equal(testOb->query_allow_static_pages(), 1),

        testOb->allow_static_pages(0),
        assert_equal(testOb->query_allow_static_pages(), 0),
    }) :));
}

void test_response () {
    expect("response is formatted", (: ({
        assert_equal(testOb->format_response(([ "code": "code", "connection": "connection", "type": "type", "content": "content" ])), "HTTP/1.0 code\r\nConnection: connection\r\nServer: lpc-http 0.1\r\nContent-Type: type\r\n\r\ncontent")
    }) :));
}