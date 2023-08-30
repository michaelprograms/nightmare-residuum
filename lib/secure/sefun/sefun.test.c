inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/sefun.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_version () {
    expect_function("version", testOb);
    expect("version returns a version", (: ({
        assert(stringp(testOb->version()), "==", 1),
        assert(strlen(testOb->version()) > 0, "==", 1),
    }) :));
}
void test_mudlib_version () {
    expect_function("mudlib_version", testOb);
    expect("mudlib_version returns a version", (: ({
        assert(stringp(testOb->mudlib_version()), "==", 1),
        assert(strlen(testOb->mudlib_version()) > 0, "==", 1),
    }) :));
}
void test_mud_name () {
    expect_function("mud_name", testOb);
    expect("mud_name returns a name", (: ({
        assert(stringp(testOb->mud_name()), "==", 1),
        assert(strlen(testOb->mud_name()) > 0, "==", 1),
    }) :));
}
void test_driver_port () {
    expect_function("driver_port", testOb);
    expect("driver_port returns a port", (: ({
        assert(intp(testOb->driver_port()), "==", 1),
        assert(testOb->driver_port() > 0, "==", 1),
    }) :));
}
void test_call_trace () {
    expect_function("call_trace", testOb);
    expect("call_trace returns a port", (: ({
        assert(regexp(testOb->call_trace(), "/secure/sefun/sefun.test#[0-9]+"), "==", 1),
    }) :));
}