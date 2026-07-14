inherit M_TEST;

/**
 * @var {"/std/resource/resource"} testOb
 */

void test_resource () {
    expect("resource is cloneable", (: ({
        assert_equal(clonep(testOb), 1),
        assert_equal(testOb->query_name(), "resource"),
        assert_equal(testOb->is_resource(), 1),
    }) :));
}

void test_types () {
    expect("resource types are queryable", (: ({
        assert_equal(sizeof(testOb->query_types()), 4),
        assert_equal(member_array("ore", testOb->query_types()) > -1, 1),
        assert_equal(member_array("wood", testOb->query_types()) > -1, 1),
        assert_equal(member_array("hide", testOb->query_types()) > -1, 1),
        assert_equal(member_array("salvage", testOb->query_types()) > -1, 1),
    }) :));
}

void test_material () {
    expect("query_material maps levels to tiered names", (: ({
        // tier = (level + 1) / 2, clamped 1..10
        assert_equal(testOb->query_material("ore", 1), "aluminum"),
        assert_equal(testOb->query_material("ore", 2), "aluminum"),
        assert_equal(testOb->query_material("ore", 3), "tin"),
        assert_equal(testOb->query_material("ore", 11), "silver"),
        assert_equal(testOb->query_material("ore", 20), "titanium"),
        assert_equal(testOb->query_material("ore", 999), "titanium"),
        assert_equal(testOb->query_material("ore", 0), "aluminum"),
        assert_equal(testOb->query_material("wood", 1), "balsa"),
        assert_equal(testOb->query_material("hide", 5), "coarse"),
        assert_equal(testOb->query_material("salvage", 9), "fasteners"),
        assert_equal(testOb->query_material("nothing", 1), 0),
    }) :));
}

void test_set_type () {
    expect("set_type rejects unknown types", (: ({
        assert_catch((: testOb->set_type("nothing") :), "*Bad argument 1 to resource->set_type\n"),
        assert_catch((: testOb->set_type(0) :), "*Bad argument 1 to resource->set_type\n"),
    }) :));
}

void test_descriptions () {
    expect("set_type and set_level name the resource", (: ({
        testOb->set_type("ore"),
        testOb->set_level(5),
        assert_equal(testOb->query_name(), "copper ore"),
        assert_equal(testOb->query_short(), "a chunk of copper ore"),
        assert_equal(testOb->query_long(), "A chunk containing a strip of copper ore."),
        assert_equal(testOb->id("ore"), 1),
        assert_equal(testOb->id("copper"), 1),

        testOb->set_type("wood"),
        assert_equal(testOb->query_name(), "cedar wood"),
        assert_equal(testOb->query_short(), "a log of cedar wood"),

        testOb->set_type("hide"),
        testOb->set_level(1),
        assert_equal(testOb->query_name(), "ragged hide"),
        assert_equal(testOb->query_short(), "a ragged hide"),
        assert_equal(testOb->query_long(), "A ragged hide stripped from a creature."),
        assert_equal(testOb->id("hide"), 1),

        testOb->set_type("salvage"),
        testOb->set_level(20),
        assert_equal(testOb->query_name(), "salvaged alloy"),
        assert_equal(testOb->query_short(), "a piece of salvaged alloy"),
        assert_equal(testOb->query_long(), "A piece of alloy salvaged from a structure."),
        assert_equal(testOb->id("salvage"), 1),
    }) :));
}
