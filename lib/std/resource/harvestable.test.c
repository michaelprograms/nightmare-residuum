inherit M_TEST;
inherit STD_OBJECT;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/resource/harvestable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_direct_harvest_obj () {
    object r = new(STD_ROOM);

    expect("direct_harvest_obj returns true", (: ({
        // check initial lack of environments
        assert(environment(testOb), "==", 0),
        assert(environment(), "==", 0),
        assert(testOb->direct_harvest_obj(), "==", 0),  // failure

        // check mismatch environments
        assert(handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(testOb->handle_move($(r)), "==", 1),
        assert(testOb->direct_harvest_obj(), "==", 0),  // failure

        // check matching environments
        assert(handle_move($(r)), "==", 1),
        assert(testOb->direct_harvest_obj(), "==", 1),  // success

        // cleanup
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
    }) :));

    if (r) destruct(r);
}