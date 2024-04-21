inherit M_TEST;
inherit STD_OBJECT;

void test_direct_harvest_obj () {
    object r = new(STD_ROOM);

    expect("direct_harvest_obj returns true", (: ({
        // check initial lack of environments
        assert_equal(environment(testOb), 0),
        assert_equal(environment(), 0),
        assert_equal(testOb->direct_harvest_obj(), 0),  // failure

        // check mismatch environments
        assert_equal(handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(testOb->handle_move($(r)), 1),
        assert_equal(testOb->direct_harvest_obj(), 0),  // failure

        // check matching environments
        assert_equal(handle_move($(r)), 1),
        assert_equal(testOb->direct_harvest_obj(), 1),  // success

        // cleanup
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(this_object()->handle_move("/domain/Nowhere/room/void.c"), 1),
    }) :));

    if (r) destruct(r);
}

void test_type () {
    expect("type is queryable and settable", (: ({
        assert_equal(testOb->query_type(), UNDEFINED),

        // invalid type
        testOb->set_type("nothing"),
        assert_equal(testOb->query_type(), UNDEFINED),

        // valid type
        testOb->set_type("ore"),
        assert_equal(testOb->query_type(), "ore"),
    }) :));
}

void test_level () {
    expect("set_level adjusts descriptions", (: ({
        // no type set
        assert_equal(testOb->query_short(), "a resource node"),
        assert_equal(testOb->query_long(), "A resource node."),

        // still no type set
        testOb->set_level(1),
        assert_equal(testOb->query_short(), "a resource node"),
        assert_equal(testOb->query_long(), "A resource node."),

        // type set
        testOb->set_type("ore"),
        testOb->set_level(1),
        assert_equal(testOb->query_short(), "a rock containing aluminum ore"),
        assert_equal(testOb->query_long(), "A rock containing a strip of aluminum ore."),
    }) :));
}