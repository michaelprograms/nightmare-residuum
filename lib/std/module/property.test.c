inherit M_TEST;

/**
 * @var {"/std/module/property"} testOb
 */

string *test_order () {
    return ({ "test_property_single", "test_properties_multiple", });
}

void test_null_properties () {
    expect("null achievements are initialized", (: ({
        assert_equal(testOb->query_properties(), ([ ])),
        store_variable("__Properties", UNDEFINED, testOb),
        assert_equal(testOb->query_properties(), ([ ])),
    }) :));
}

void test_property_single () {
    expect("add, set, remove, and query property are handled", (: ({
        assert_equal(testOb->query_property("test_key"), UNDEFINED),

        assert_equal(testOb->set_property("test_key1", "test_value"), "test_value"),
        assert_equal(testOb->query_property("test_key1"), "test_value"),

        assert_equal(testOb->set_property("test_key2", to_float(123)), to_float(123)),
        assert_equal(testOb->query_property("test_key2"), to_float(123)),

        assert_equal(testOb->set_property("test_key4", ""), ""),
        assert_equal(testOb->query_property("test_key4"), ""),

        assert_equal(testOb->add_property("test_key3", 10), 10),
        assert_equal(testOb->query_property("test_key3"), 10),
        assert_equal(testOb->add_property("test_key3", 1), 11),
        assert_equal(testOb->query_property("test_key3"), 11),

        assert_equal(testOb->remove_property("test_key1"), 1),
        assert_equal(testOb->query_property("test_key1"), UNDEFINED),
        assert_equal(testOb->remove_property("test_key2"), 1),
        assert_equal(testOb->query_property("test_key2"), UNDEFINED),
    }) :));
    expect("add, set, remove, and query property handle bad inputs", (: ({
        assert_catch((: testOb->query_property(UNDEFINED) :), "*Bad argument 1 to property->query_property\n"),

        assert_catch((: testOb->set_property(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to property->set_property\n"),
        assert_catch((: testOb->set_property("key", UNDEFINED) :), "*Bad argument 2 to property->set_property\n"),
        assert_catch((: testOb->set_property("key", ({ })) :), "*Bad argument 2 to property->set_property\n"),
        assert_catch((: testOb->set_property("key", ([ ])) :), "*Bad argument 2 to property->set_property\n"),

        assert_catch((: testOb->add_property(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to property->add_property\n"),
        assert_catch((: testOb->add_property("key", UNDEFINED) :), "*Bad argument 2 to property->add_property\n"),
        assert_catch((: testOb->add_property("key", ({ })) :), "*Bad argument 2 to property->add_property\n"),
        assert_catch((: testOb->add_property("key", ([ ])) :), "*Bad argument 2 to property->add_property\n"),

        assert_catch((: testOb->set_properties(UNDEFINED) :), "*Bad argument 1 to property->set_properties\n"),

        assert_catch((: testOb->remove_property(UNDEFINED) :), "*Bad argument 1 to property->remove_property\n"),
    }) :));
}

private mapping m1 = ([ "test_key1": "test_value" ]);
private mapping m2 = ([ "test_key2": to_float(123) ]);

void test_properties_multiple () {
    expect("set and query properties are handled", (: ({
        assert_equal(testOb->query_properties(), ([ ])),
        assert_equal(testOb->set_properties(m1), identify(m1)),
        assert_equal(testOb->query_properties(), identify(m1)),
        assert_equal(testOb->set_properties(m2), identify(m1+m2)),
        assert_equal(testOb->query_properties(), identify(m1+m2)),
        assert_equal(testOb->remove_property("test_key2"), 1),
        assert_equal(testOb->query_properties(), identify(m1)),
        assert_equal(testOb->remove_property("test_key1"), 1),
        assert_equal(testOb->query_properties(), ([ ])),
    }) :));

    expect("remove properties is handled", (: ({
        assert_equal(testOb->remove_properties(), ([ ])),
        assert_equal(testOb->query_properties(), ([ ])),
        assert_equal(testOb->set_properties(m1), identify(m1)),
        assert_equal(testOb->set_properties(m2), identify(m1+m2)),
        assert_equal(testOb->remove_properties(), ([ ])),
        assert_equal(testOb->query_properties(), ([ ])),
    }) :));
}