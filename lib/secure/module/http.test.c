inherit M_TEST;

/**
 * @var {"/secure/module/http"} testOb
 */

string *test_order() {
    return ({
        "test_config",
        "test_response",
        "test_parse_request",
        "test_handle_response",
        "test_binary_response",
        "test_static_and_404",
        "test_server",
        "test_sockets",
    });
}

void test_config() {
    expect("port is queryable and settable", (: ({
        assert_equal(testOb->query_port(), 0),

        testOb->set_port(123),
        assert_equal(testOb->query_port(), 123),
    }) :));

    expect("url patterns are queryable and settable", (: ({
        assert_equal(testOb->query_url_patterns(), ({})),

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

void test_response() {
    expect("response is formatted", (: ({
        assert_equal(
            testOb->format_response(([
                "code": "code",
                "connection": "connection",
                "type": "type",
                "content": "content"
            ])),
            "HTTP/1.0 code\r\nConnection: connection\r\nServer: lpc-http 0.1\r\nContent-Type: type\r\n\r\ncontent"
        )
    }) :));
}

void test_parse_request() {
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

    req = testOb->parse_request("TRACE /res HTTP/1.0\r\n\r\n");
    expect("parses TRACE method", (: ({
        assert_equal($(req["method"]), 6),
    }) :));

    req = testOb->parse_request("CONNECT /res HTTP/1.0\r\n\r\n");
    expect("parses CONNECT method", (: ({
        assert_equal($(req["method"]), 7),
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

void test_handle_response() {
    mapping res;

    res = testOb->handle_response(([]), "a.b.c.d", ({}));
    expect("default case (4+ dot segments) returns 500", (: ({
        assert_equal($(res["code"]), "500 Internal Server Error"),
        assert_equal($(res["connection"]), "close"),
    }) :));
}

void test_binary_response() {
    mixed out;

    out = testOb->format_response(([
        "code": "200 OK",
        "connection": "close",
        "type": "image/png",
        "content": to_buffer("PNGDATA"),
    ]));
    expect("format_response returns a buffer for buffer content", (: ({
        assert_equal(bufferp($(out)), 1),
        assert_regex(string_decode($(out), "UTF-8"), "Content-Type: image/png"),
        assert_regex(string_decode($(out), "UTF-8"), "PNGDATA"),
    }) :));
}

void test_static_and_404() {
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


/* ----- server ----- */

// A server written to /tmp inherits the instrumented copy, so its server_root
// is /tmp/ and the handler files and static pages below can live there too.
#define SERVER "/tmp/http_server.c"
#define HANDLER "/tmp/http_handler.c"
#define DIR_HANDLER "/tmp/http_dir/handler.c"
#define PAGE "/tmp/www/http_page.html"

/** @type {"/secure/module/http"} */
nosave private object __Server;
/** @type {"/daemon/log.mock"} */
nosave private object __Log;

string query_last_log() {
    return __Log->query_logs()[<1][1];
}

void write_server_files() {
    write_file(SERVER, "inherit \"/secure/module/http.coverage.c\";\n"
        "mixed text(mapping res, string *args) { return args; }\n"
        "buffer binary(mapping res, string *args) { return to_buffer(\"BIN\"); }\n"
        "buffer typed(mapping res, string *args) {\n"
        "    res[\"type\"] = \"image/png\";\n"
        "    return to_buffer(\"PNG\");\n"
        "}\n", 1);
    write_file(
        HANDLER,
        "int data(mapping res, string *args) { return 2; }\n",
        1
    );
    mkdir("/tmp/http_dir");
    write_file(
        DIR_HANDLER,
        "int data(mapping res, string *args) { return 3; }\n",
        1
    );
    mkdir("/tmp/www");
    write_file(PAGE, "<p>page</p>", 1);
}
void remove_server_files() {
    foreach (string file in ({ SERVER, HANDLER, DIR_HANDLER, PAGE })) {
        if (find_object(file)) {
            destruct(find_object(file));
        }
        rm(file);
    }
    rmdir("/tmp/http_dir");
    rmdir("/tmp/www");
}

void test_server() {
    write_server_files();
    // @lpc-ignore
    __Server = new(SERVER);
    __Server->add_url_pattern("^/text/(\\w+)", "text");
    __Server->add_url_pattern("^/binary", "binary");
    __Server->add_url_pattern("^/typed", "typed");

    expect("handle_response resolves handlers and encodes results", (: ({
        assert_equal(
            __Server->handle_response(([]), "text", ({ "a" })),
            ([
                "content": "[\"a\"]",
                "type": "text/json",
                "code": "200 OK",
                "connection": "close",
            ])
        ),
        assert_equal(
            __Server->handle_response(([]), "binary", ({})),
            ([
                "content": to_buffer("BIN"),
                "type": "application/octet-stream",
                "code": "200 OK",
                "connection": "close",
            ])
        ),
        assert_equal(
            __Server->handle_response(([]), "typed", ({}))["type"],
            "image/png"
        ),
        assert_equal(
            __Server->handle_response(
                ([]),
                "http_handler.data",
                ({})
            )["content"],
            "2"
        ),
        assert_equal(
            __Server->handle_response(
                ([]),
                "http_dir.handler.data",
                ({})
            )["content"],
            "3"
        ),
    }) :));

    // read_socket logs each response, so record D_LOG instead of writing
    __Log = new("/daemon/log.mock.c");
    __Log->start_shadow(load_object(D_LOG));

    expect("read_socket handles matched, static, and unmatched requests", (: ({
        // matched pattern
        __Server->read_socket(-1, "GET /text/a HTTP/1.0\r\n\r\n"),
        assert_regex(query_last_log(), "\\) 200 OK text/json 5 bytes$"),

        // static pages: served, missing, and parent paths rejected
        __Server->allow_static_pages(1),
        __Server->read_socket(-1, "GET /http_page.html HTTP/1.0\r\n\r\n"),
        assert_regex(query_last_log(), "\\) 200 OK text/html 11 bytes$"),
        __Server->read_socket(-1, "GET /missing.html HTTP/1.0\r\n\r\n"),
        assert_regex(query_last_log(), "\\) 404 Not Found text/html"),
        __Server->read_socket(-1, "GET /../http_page.html HTTP/1.0\r\n\r\n"),
        assert_regex(query_last_log(), "\\) 404 Not Found text/html"),
    }) :));

    __Log->stop_shadow();
    destruct(__Log);

    if (__Server) {
        destruct(__Server);
    }
    remove_server_files();
}

void test_sockets() {
    write_server_files();
    // @lpc-ignore
    __Server = new(SERVER);

    expect("socket handlers fail without socket access", (: ({
        assert_catch((: __Server->start() :), "*http: No port configured.\n"),

        // only real inheritors of the HTTP module are allowed sockets
        __Server->set_port(16670),
        assert_regex(
            catch(__Server->start()),
            "^\\*Bad response socket_create: "
        ),
        assert_catch(
            (: __Server->listen_socket(-1) :),
            "*http: Couldn't perform socket_accept.\n"
        ),

        __Server->write_socket(-1),
        __Server->close_socket(-1),
        __Server->handle_remove(),
    }) :));

    if (__Server) {
        destruct(__Server);
    }
    remove_server_files();
}
