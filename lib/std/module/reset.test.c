inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/reset.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

private int resetFnCalled = 0;
void test_resets () {
    function resetFn = function () {
        resetFnCalled ++;
    };

    expect_function("query_reset", testOb);
    expect_function("query_resets", testOb);
    expect_function("set_reset", testOb);

    expect("resets handle setting, querying, and resetting", (: ({
        // have not set_reset yet
        assert(testOb->query_resets(), "==", 0),
        assert(testOb->query_reset(), "==", ([ ])),

        // set_reset will call reset
        testOb->set_reset(([ "/std/item.c": 1])),
        assert(testOb->query_resets(), "==", 1),
        assert(testOb->query_reset(), "==", ([ "/std/item.c": 1 ])),
        testOb->reset(),
        assert(testOb->query_resets(), "==", 2),

        // set_reset will call reset
        testOb->set_reset(([ "/std/item.c": $(resetFn)])),
        assert(testOb->query_resets(), "==", 3),
        assert(testOb->query_reset(), "==", ([ "/std/item.c": $(resetFn) ])),
        assert(resetFnCalled, "==", 1),
        testOb->reset(),
        assert(testOb->query_resets(), "==", 4),
        assert(resetFnCalled, "==", 2),
    }) :));
}