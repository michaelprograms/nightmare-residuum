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
    expect_function("reset_pickable", testOb);

    expect("handles initializing with zeroes", (: ({
        assert(testOb->query_picks(), "==", 0),
        assert(testOb->query_max_picks(), "==", 0),
        assert(testOb->query_pick_message(), "==", UNDEFINED),
    }) :));

    expect("handles setting item, picks, and message", (: ({
        // set at 2 picks
        testOb->set_pickable(STD_ITEM, 2, "An item is pickable."),
        assert(testOb->query_pick_item(), "==", STD_ITEM),
        assert(testOb->query_picks(), "==", 2),
        assert(testOb->query_max_picks(), "==", 2),
        assert(testOb->query_pick_message(), "==", "An item is pickable."),
    }) :));

    expect("handles pick and limits", (: ({
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

    expect("handles resetting pick", (: ({
        // reset pick
        testOb->reset_pickable(),
        assert(testOb->query_picks(), "==", 1),
    }) :));

    expect("set_pickable handles invalid argument 1", (: ({
        assert((: testOb->set_pickable(this_object()) :), "catch", "*Bad argument 1 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable(1) :), "catch", "*Bad argument 1 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable(1.0) :), "catch", "*Bad argument 1 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable(({})) :), "catch", "*Bad argument 1 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable(([])) :), "catch", "*Bad argument 1 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable((: 1 :)) :), "catch", "*Bad argument 1 to pickable->set_pickable\n"),
    }) :));

    expect("set_pickable handles invalid argument 2", (: ({
        assert((: testOb->set_pickable("", this_object()) :), "catch", "*Bad argument 2 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", "") :), "catch", "*Bad argument 2 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", ({})) :), "catch", "*Bad argument 2 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", ([])) :), "catch", "*Bad argument 2 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", (: 1 :)) :), "catch", "*Bad argument 2 to pickable->set_pickable\n"),
    }) :));

    expect("set_pickable handles invalid argument 3", (: ({
        assert((: testOb->set_pickable("", 1, this_object()) :), "catch", "*Bad argument 3 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", 1, 1) :), "catch", "*Bad argument 3 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", 1, 1.0) :), "catch", "*Bad argument 3 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", 1, ({})) :), "catch", "*Bad argument 3 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", 1, ([])) :), "catch", "*Bad argument 3 to pickable->set_pickable\n"),
        assert((: testOb->set_pickable("", 1, (: 1 :)) :), "catch", "*Bad argument 3 to pickable->set_pickable\n"),
    }) :));
}

void test_apply_pick_obj () {
    expect_function("direct_pick_obj", testOb);

    expect("direct_pick_obj returns true when same environment", (: ({
        // true with same environment (no env)
        assert(testOb->direct_pick_obj(this_object()), "==", 1),
        // false with different environments (one env)
        assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(testOb->direct_pick_obj(this_object()), "==", 0),
        // false with no object sent
        assert(testOb->direct_pick_obj(), "==", 0),
    }) :));
}