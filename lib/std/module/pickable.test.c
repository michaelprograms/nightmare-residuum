inherit M_TEST;
inherit M_MOVE;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/pickable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_pickable () {
    expect_function("query_picks", testOb);
    expect_function("query_max_picks", testOb);
    expect_function("query_pick_message", testOb);
    expect_function("set_pickable", testOb);
    expect_function("query_pick_item", testOb);
    expect_function("handle_pick", testOb);

    expect("handles item, max, and message parameters", (: ({
        assert(testOb->query_picks(), "==", 0),
        assert(testOb->query_max_picks(), "==", 0),
        assert(testOb->query_pick_message(), "==", 0),

        // set at 2 picks
        testOb->set_pickable(STD_ITEM, 2, "An item is pickable."),
        assert(testOb->query_pick_item(), "==", STD_ITEM),
        assert(testOb->query_max_picks(), "==", 2),
        assert(testOb->query_pick_message(), "==", "An item is pickable."),
        assert(testOb->query_picks(), "==", 2),

        // pick 1st
        assert(testOb->handle_pick(), "==", 1),
        assert(testOb->query_picks(), "==", 1),

        // pick 2nd
        assert(testOb->handle_pick(), "==", 1),
        assert(testOb->query_picks(), "==", 0),

        // can't pick
        assert(testOb->handle_pick(), "==", 0),
        assert(testOb->query_picks(), "==", 0),
    }) :));
}