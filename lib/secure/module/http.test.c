inherit M_TEST;

/**
 * @var {"/secure/module/http"} testOb
 */

string *test_order () {
    return ({ "test_config", "test_response", "test_parse_request", "test_handle_response", "test_static_and_404" });
}

void test_config () {
    expect("port is queryable and settable", (: ({
        assert_equal(testOb->query_port(), 0),

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

void test_parse_request () {
    mapping req;

    req = testOb->parse_request("GET /foo HTTP/1.0\r\n\r\n");
    expect("parses GET request line", (: ({
        assert_equal($(req["method"]), 1),
        assert_equal($(req["uri"]), "/foo"),
        assert_equal($(req["protocol"]), "HTTP/1.0"),
    }) :));

    req = testOb->parse_request("POST /submit HTTP/1.0\r\n\r\n");
    expect("parses POST method", (: ({
        assert_equal($(req["method"]), 2),
    }) :));

    req = testOb->parse_request("HEAD / HTTP/1.0\r\n\r\n");
    expect("parses HEAD method", (: ({
        assert_equal($(req["method"]), 3),
    }) :));

    req = testOb->parse_request("PUT /res HTTP/1.0\r\n\r\n");
    expect("parses PUT method", (: ({
        assert_equal($(req["method"]), 4),
    }) :));

    req = testOb->parse_request("DELETE /res HTTP/1.0\r\n\r\n");
    expect("parses DELETE method", (: ({
        assert_equal($(req["method"]), 5),
    }) :));

    req = testOb->parse_request("OPTIONS / HTTP/1.0\r\n\r\n");
    expect("unknown method parses as UNDEF_REQUEST", (: ({
        assert_equal($(req["method"]), -1),
    }) :));

    req = testOb->parse_request("GET / HTTP/1.0\r\nHost: localhost\r\nAccept: */*\r\n\r\n");
    expect("parses headers", (: ({
        assert_equal($(req["header"]["Host"]), "localhost"),
        assert_equal($(req["header"]["Accept"]), "*/*"),
    }) :));

    req = testOb->parse_request("POST /submit HTTP/1.0\r\nContent-Type: text/plain\r\n\r\nhello");
    expect("parses body content", (: ({
        assert_equal($(req["content"]), "hello"),
    }) :));

    req = testOb->parse_request("GET / HTTP/1.0\r\nBadHeaderLine\r\n\r\n");
    expect("handles malformed header without colon", (: ({
        assert_equal(mapp($(req["header"])), 1),
        assert_equal(sizeof($(req["header"])), 0),
    }) :));

    req = testOb->parse_request("GET / HTTP/1.0\r\n\r\nHost: localhost\r\n\r\n");
    expect("handles blank header line without error", (: ({
        assert_equal(mapp($(req["header"])), 1),
    }) :));
}

void test_handle_response () {
    mapping res;

    res = testOb->handle_response(([ ]), "a.b.c.d", ({ }));
    expect("default case (4+ dot segments) returns 500", (: ({
        assert_equal($(res["code"]), "500 Internal Server Error"),
        assert_equal($(res["connection"]), "close"),
    }) :));
}

void test_static_and_404 () {
    string out;

    out = testOb->format_response(([
        "code": "404 Not Found",
        "connection": "close",
        "type": "text/html",
        "content": "Not Found",
    ]));
    expect("format_response builds valid 404 response", (: ({
        assert_regex($(out), "HTTP/1.0 404 Not Found"),
        assert_regex($(out), "Connection: close"),
        assert_regex($(out), "Content-Type: text/html"),
        assert_regex($(out), "Not Found"),
    }) :));
}
