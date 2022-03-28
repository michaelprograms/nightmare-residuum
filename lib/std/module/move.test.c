inherit M_TEST;
inherit STD_CONTAINER;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/move.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_ignore () {
    return ::test_ignore() + ({ "can_receive", "can_release", "handle_receive", "handle_release", });
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
    object ob = new(STD_CONTAINER), ob2 = new(STD_CONTAINER);

    expect_function("handle_move", testOb);

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
        assert(canReceiveCount, "==", 0),
        assert(handleReceiveCount, "==", 0),
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(canReceiveCount, "==", 1),
        assert(handleReceiveCount, "==", 1),

        assert(canReleaseCount, "==", 0),
        assert(handleReleaseCount, "==", 0),
        assert(testOb->handle_move($(ob)), "==", 1), // move to test container
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

        // test can_release
        noReceive = 0,
        assert(testOb->handle_move(this_object()), "==", 1),
        noRelease = 1,
        assert(canReleaseCount, "==", 0),
        assert(handleReleaseCount, "==", 0),
        assert(testOb->handle_move($(ob)), "==", 0), // attempt to move to test container
        assert(canReleaseCount, "==", 1), // can_release was called
        assert(handleReleaseCount, "==", 0), // handle_release was not called
    }) :));

    destruct(ob);
    destruct(ob2);
}

void test_environment () {
    expect_function("query_environment_path", testOb);
    expect_function("query_environment_short", testOb);
    expect_function("set_environment_path", testOb);

    expect("handles setting and querying environment path and short", (: ({
        assert(testOb->query_environment_path(), "==", "/domain/Nowhere/room/void"), // defaults to void
        assert(testOb->query_environment_short(), "==", "no where"),
        assert(testOb->set_environment_path("/domain/Nowhere/room/freezer"), "==", 0),
        assert(testOb->query_environment_path(), "==", "/domain/Nowhere/room/freezer"),
        assert(testOb->query_environment_short(), "==", "a freezer"),
    }) :));

    expect("set_environment_path handles invalid argument 1", (: ({
        assert((: testOb->set_environment_path(0) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path(0.0) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path("") :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path(({})) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path(([])) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path((: users :)) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
    }) :));
}