inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/reset.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

private int resetFnCalled = 0;
private int setupFnCalled = 0;
void test_resets () {
    function setupFn = function () {
        setupFnCalled ++;
        return 0;
    };

    expect_function("query_reset", testOb);
    expect_function("query_resets", testOb);
    expect_function("set_reset", testOb);
    expect_function("set_reset_data", testOb);
    expect_function("handle_reset", testOb);

    expect("resets handle setting, querying, and resetting", (: ({
        // have not set_reset yet
        assert(testOb->query_resets(), "==", 0),
        assert(testOb->query_reset(), "==", ([ ])),

        // set_reset will call reset for integer argument
        testOb->set_reset(([ "/std/item.c": 1 ])),
        assert(testOb->query_resets(), "==", 1),
        assert(testOb->query_reset(), "==", ([ "/std/item.c": 1 ])),
        testOb->reset(),
        assert(testOb->query_resets(), "==", 2),

        // set_reset will call reset for map argument
        testOb->set_reset(([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert(testOb->query_resets(), "==", 3),
        assert(testOb->query_reset(), "==", ([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert(setupFnCalled, "==", 1),
        testOb->reset(),
        assert(testOb->query_resets(), "==", 4),
        assert(setupFnCalled, "==", 2),

        // set_reset_data will not call reset for integer argument
        testOb->set_reset_data(([ "/std/item.c": 1 ])),
        assert(testOb->query_resets(), "==", 4),
        assert(testOb->query_reset(), "==", ([ "/std/item.c": 1 ])),
        // set_reset_data will not call reset for map argument
        testOb->set_reset_data(([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert(testOb->query_resets(), "==", 4),
        assert(testOb->query_reset(), "==", ([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
    }) :));
}