inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/sefun");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_driver_version () {
    expect_function("driver_version", testOb);
    expect("driver_version returns a version", (: ({
        assert(stringp(testOb->driver_version()), "==", 1),
        assert(strlen(testOb->driver_version()) > 0, "==", 1),
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