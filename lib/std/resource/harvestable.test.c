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

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("type is queryable and settable", (: ({
        assert(testOb->query_type(), "==", UNDEFINED),

        // invalid type
        testOb->set_type("nothing"),
        assert(testOb->query_type(), "==", UNDEFINED),

        // valid type
        testOb->set_type("ore"),
        assert(testOb->query_type(), "==", "ore"),
    }) :));
}

void test_level () {
    expect("set_level adjusts descriptions", (: ({
        // no type set
        assert(testOb->query_short(), "==", "a resource node"),
        assert(testOb->query_long(), "==", "A resource node."),

        // still no type set
        testOb->set_level(1),
        assert(testOb->query_short(), "==", "a resource node"),
        assert(testOb->query_long(), "==", "A resource node."),

        // type set
        testOb->set_type("ore"),
        testOb->set_level(1),
        assert(testOb->query_short(), "==", "a rock containing aluminum ore"),
        assert(testOb->query_long(), "==", "A rock containing a strip of aluminum ore."),
    }) :));
}