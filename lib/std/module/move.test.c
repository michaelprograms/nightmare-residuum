inherit M_TEST;
inherit M_CONTAINER;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/move.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

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

void test_move () {
    object ob = new(M_CONTAINER), ob2 = new(M_CONTAINER);

    expect_function("handle_move", testOb);
    expect_function("handle_received", testOb);
    expect_function("handle_released", testOb);

    expect("handle_move relocates objects", (: ({
        assert(sizeof(all_inventory($(ob))), "==", 0),
        assert(testOb->handle_move($(ob)), "==", 1),
        assert(sizeof(all_inventory($(ob))), "==", 1),
        assert(sizeof(all_inventory($(ob2))), "==", 0),
        assert(testOb->handle_move($(ob2)), "==", 1),
        assert(sizeof(all_inventory($(ob2))), "==", 1),
        assert(sizeof(all_inventory($(ob))), "==", 0),
    }) :));

    expect("handle_move calls release and receive hooks", (: ({
        // reset counts
        canReceiveCount = 0,
        handleReceiveCount = 0,
        canReleaseCount = 0,
        handleReleaseCount = 0,

        assert(testOb->handle_move(this_object()), "==", 1),
        // handle_received called handle_receive
        assert(canReceiveCount, "==", 1),
        assert(handleReceiveCount, "==", 1),

        assert(canReleaseCount, "==", 0),
        assert(handleReleaseCount, "==", 0),
        assert(testOb->handle_move($(ob)), "==", 1), // move to test container
        // handle_released called handle_release
        assert(canReleaseCount, "==", 1),
        assert(handleReleaseCount, "==", 1),

        // reset counts
        canReceiveCount = 0,
        handleReceiveCount = 0,
        canReleaseCount = 0,
        handleReleaseCount = 0,

        // test can_receive
        noReceive = 1,
        assert(canReceiveCount, "==", 0),
        assert(handleReceiveCount, "==", 0),
        assert(testOb->handle_move(this_object()), "==", 0),
        assert(canReceiveCount, "==", 1), // can_receive was called
        assert(handleReceiveCount, "==", 0), // handle_receive was not called
        noReceive = 0,

        // test can_release
        assert(testOb->handle_move(this_object()), "==", 1),
        noRelease = 1,
        assert(canReleaseCount, "==", 0),
        assert(handleReleaseCount, "==", 0),
        assert(testOb->handle_move($(ob)), "==", 0), // attempt to move to test container
        assert(canReleaseCount, "==", 1), // can_release was called
        assert(handleReleaseCount, "==", 0), // handle_release was not called
        noRelease = 0,
    }) :));

    destruct(ob);
    destruct(ob2);
}

void test_environment () {
    expect_function("query_environment_path", testOb);
    expect_function("query_environment_short", testOb);
    expect_function("set_environment_path", testOb);

    expect("handles setting and querying environment path and short", (: ({
        assert(testOb->query_environment_path(), "==", "/domain/Nowhere/room/void.c"), // defaults to void
        assert(testOb->query_environment_short(), "==", "no where"),

        // set to freezer (this can't happen normally)
        testOb->set_environment_path("/domain/Nowhere/room/freezer.c"),
        assert(testOb->query_environment_path(), "==", "/domain/Nowhere/room/freezer.c"),
        assert(testOb->query_environment_short(), "==", "a freezer"),

        // move to this_object
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(testOb->query_environment_path(), "==", base_name() + ".c"),

        // moves to void & freezer but env path is still this_object
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(testOb->query_environment_path(), "==", base_name() + ".c"),
        assert(testOb->handle_move("/domain/Nowhere/room/freezer.c"), "==", 1),
        assert(testOb->query_environment_path(), "==", base_name() + ".c"),
    }) :));
}