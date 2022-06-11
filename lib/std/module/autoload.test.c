inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/autoload.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

nosave private int __MockLevel;
int query_level () { return __MockLevel; }

void test_autoload () {
    expect_function("set_autoload", testOb);
    expect_function("query_autoload_data", testOb);
    expect_function("query_autoload", testOb);
    expect_function("restore_autoload", testOb);

    expect("set_autoload should set requirements", (: ({
        __MockLevel = 0,
        assert(arrayp(testOb->query_autoload(this_object())), "==", 1),
        testOb->set_autoload(1),
        assert(arrayp(testOb->query_autoload(this_object())), "==", 0),
        __MockLevel = 1,
        assert(arrayp(testOb->query_autoload(this_object())), "==", 1),
    }) :));

    expect("query_autoload_data should return an array", (: ({
        assert(testOb->query_autoload_data(), "==", ({ })),
    }) :));

    expect("restore_autoload does nothing", (: ({
        assert(testOb->restore_autoload(({ })), "==", UNDEFINED),
    }) :));
}