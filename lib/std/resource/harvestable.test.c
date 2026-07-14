inherit M_TEST;
inherit STD_OBJECT;

/**
 * @var {"/std/resource/harvestable"} testOb
 */

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
        assert_equal(testOb->query_type(), ""),

        // invalid types
        testOb->set_type("nothing"),
        assert_equal(testOb->query_type(), ""),
        testOb->set_type("hide"), // hide drops from NPCs, never a node
        assert_equal(testOb->query_type(), ""),

        // valid types
        testOb->set_type("ore"),
        assert_equal(testOb->query_type(), "ore"),
        testOb->set_type("salvage"),
        assert_equal(testOb->query_type(), "salvage"),
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

        // ore type: level 1 is tier 1
        testOb->set_type("ore"),
        testOb->set_level(1),
        assert_equal(testOb->query_short(), "a rock containing aluminum ore"),
        assert_equal(testOb->query_long(), "A rock containing a strip of aluminum ore."),

        // tier mapping: level 3 is tier 2, level 20 is tier 10
        testOb->set_level(3),
        assert_equal(testOb->query_short(), "a rock containing tin ore"),
        testOb->set_level(20),
        assert_equal(testOb->query_short(), "a rock containing titanium ore"),

        // wood type
        testOb->set_type("wood"),
        testOb->set_level(1),
        assert_equal(testOb->query_short(), "a log containing balsa wood"),
        assert_equal(testOb->query_long(), "A log containing a strip of balsa wood."),

        // salvage type
        testOb->set_type("salvage"),
        testOb->set_level(1),
        assert_equal(testOb->query_short(), "a pile of debris containing salvaged wiring"),
        assert_equal(testOb->query_long(), "A pile of debris containing salvageable wiring."),
        assert_equal(testOb->id("salvage"), 1),
    }) :));
}