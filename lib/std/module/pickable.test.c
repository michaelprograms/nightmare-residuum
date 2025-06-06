inherit M_TEST;
inherit M_MOVE;

/**
 * @var {"/std/module/pickable"} testOb
 */

void test_pickable () {
    expect("handles initializing with zeroes", (: ({
        assert_equal(testOb->query_picks(), 0),
        assert_equal(testOb->query_max_picks(), 0),
        assert_equal(testOb->query_pick_message(), UNDEFINED),
    }) :));

    expect("handles setting item, picks, and message", (: ({
        // set at 2 picks
        testOb->set_pickable(STD_ITEM, 2, "An item is pickable."),
        assert_equal(testOb->query_pick_item(), STD_ITEM),
        assert_equal(testOb->query_picks(), 2),
        assert_equal(testOb->query_max_picks(), 2),
        assert_equal(testOb->query_pick_message(), "An item is pickable."),
    }) :));

    expect("handles pick and limits", (: ({
        // pick 1st
        assert_equal(testOb->handle_pick(), 1),
        assert_equal(testOb->query_picks(), 1),

        // pick 2nd
        assert_equal(testOb->handle_pick(), 1),
        assert_equal(testOb->query_picks(), 0),

        // can't pick
        assert_equal(testOb->handle_pick(), 0),
        assert_equal(testOb->query_picks(), 0),
    }) :));

    expect("handles resetting pick", (: ({
        // reset pick
        testOb->reset(),
        assert_equal(testOb->query_picks(), 1),
    }) :));
}

void test_apply_pick_obj () {
    expect("direct_pick_obj returns true when same environment", (: ({
        // true with same environment (no env)
        assert_equal(testOb->direct_pick_obj(this_object()), 1),
        // false with different environments (one env)
        assert_equal(this_object()->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(testOb->direct_pick_obj(this_object()), 0),
        // false with no object sent
        assert_equal(testOb->direct_pick_obj(), 0),
    }) :));
}