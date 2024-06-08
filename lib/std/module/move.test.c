inherit M_TEST;
inherit M_CONTAINER;

nosave private int canReceiveCount = 0, canReleaseCount = 0;
nosave private int handleReceiveCount = 0, handleReleaseCount = 0;
nosave private int noReceive = 0, noRelease = 0;
int can_receive (object ob) {
    canReceiveCount ++;
    if (noReceive) return 0;
    return ::can_receive(ob);
}
int can_release (object ob) {
    canReleaseCount ++;
    if (noRelease) return 0;
    return ::can_release(ob);
}
int handle_receive (object ob) {
    handleReceiveCount ++;
    return ::handle_receive(ob);
}
int handle_release (object ob) {
    handleReleaseCount ++;
    return ::handle_release(ob);
}
string query_short () {
    return "move.test.c query_short";
}

void test_move () {
    object ob = new(M_CONTAINER), ob2 = new(M_CONTAINER);

    expect("handle_move relocates objects", (: ({
        assert_equal(sizeof(all_inventory($(ob))), 0),
        assert_equal(testOb->handle_move($(ob)), 1),
        assert_equal(sizeof(all_inventory($(ob))), 1),
        assert_equal(sizeof(all_inventory($(ob2))), 0),
        assert_equal(testOb->handle_move($(ob2)), 1),
        assert_equal(sizeof(all_inventory($(ob2))), 1),
        assert_equal(sizeof(all_inventory($(ob))), 0),
    }) :));

    expect("handle_move calls release and receive hooks", (: ({
        // reset counts
        canReceiveCount = 0,
        handleReceiveCount = 0,
        canReleaseCount = 0,
        handleReleaseCount = 0,

        assert_equal(testOb->handle_move(this_object()), 1),
        // handle_received called handle_receive
        assert_equal(canReceiveCount, 1),
        assert_equal(handleReceiveCount, 1),

        assert_equal(canReleaseCount, 0),
        assert_equal(handleReleaseCount, 0),
        assert_equal(testOb->handle_move($(ob)), 1), // move to test container
        // handle_released called handle_release
        assert_equal(canReleaseCount, 1),
        assert_equal(handleReleaseCount, 1),

        // reset counts
        canReceiveCount = 0,
        handleReceiveCount = 0,
        canReleaseCount = 0,
        handleReleaseCount = 0,

        // test can_receive
        noReceive = 1,
        assert_equal(canReceiveCount, 0),
        assert_equal(handleReceiveCount, 0),
        assert_equal(testOb->handle_move(this_object()), 0),
        assert_equal(canReceiveCount, 1), // can_receive was called
        assert_equal(handleReceiveCount, 0), // handle_receive was not called
        noReceive = 0,

        // test can_release
        assert_equal(testOb->handle_move(this_object()), 1),
        noRelease = 1,
        assert_equal(canReleaseCount, 0),
        assert_equal(handleReleaseCount, 0),
        assert_equal(testOb->handle_move($(ob)), 0), // attempt to move to test container
        assert_equal(canReleaseCount, 1), // can_release was called
        assert_equal(handleReleaseCount, 0), // handle_release was not called
        noRelease = 0,
    }) :));

    destruct(ob);
    destruct(ob2);
}

void test_environment () {
    expect("handles setting and querying environment path and short", (: ({
        assert_equal(testOb->query_environment_path(), "/domain/Nowhere/room/void.c"), // defaults to void
        assert_equal(testOb->query_environment_short(), "no where"),

        // set to freezer (this can't happen normally)
        testOb->set_environment_path("/domain/Nowhere/room/freezer.c"),
        assert_equal(testOb->query_environment_path(), "/domain/Nowhere/room/freezer.c"),
        assert_equal(testOb->query_environment_short(), "a freezer"),

        // use nonexistent path
        assert_catch((: testOb->set_environment_path("/nonexistent.c") :), "*Bad argument 1 to move->set_environment_path\n"),
        assert_equal(testOb->query_dest_ob("/nonexistent.c"), 0),
        assert_catch((: testOb->handle_move("/nonexistent.c") :), "*Bad argument 1 to move->handle_move\n"),

        // move to this_object
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(testOb->query_environment_path(), base_name() + ".c"),
        assert_equal(testOb->query_environment_short(), "move.test.c query_short"),

        // moves to void & freezer but env path is still this_object
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(testOb->query_environment_path(), base_name() + ".c"),
        assert_equal(testOb->handle_move("/domain/Nowhere/room/freezer.c"), 1),
        assert_equal(testOb->query_environment_path(), base_name() + ".c"),
    }) :));
}