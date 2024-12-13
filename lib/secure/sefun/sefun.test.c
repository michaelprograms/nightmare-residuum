inherit M_TEST;

/**
 * @var {"/secure/sefun/sefun"} testOb
 */

void test_version () {
    expect("version returns a version", (: ({
        assert_equal(stringp(testOb->version()), 1),
        assert_equal(strlen(testOb->version()) > 0, 1),
    }) :));
}
void test_mudlib_version () {
    expect("mudlib_version returns a version", (: ({
        assert_equal(stringp(testOb->mudlib_version()), 1),
        assert_equal(strlen(testOb->mudlib_version()) > 0, 1),
    }) :));
}
void test_mud_name () {
    expect("mud_name returns a name", (: ({
        assert_equal(stringp(testOb->mud_name()), 1),
        assert_equal(strlen(testOb->mud_name()) > 0, 1),
    }) :));
}
void test_driver_port () {
    expect("driver_port returns a port", (: ({
        assert_equal(intp(testOb->driver_port()), 1),
        assert_equal(testOb->driver_port() > 0, 1),
    }) :));
}
void test_call_trace () {
    expect("call_trace returns a stack trace", (: ({
        assert_regex(testOb->call_trace(), "/secure/sefun/sefun.test#[0-9]+"),
    }) :));
}