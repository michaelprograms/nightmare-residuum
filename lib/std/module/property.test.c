inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/property.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({
        "test_property_bad_arguments",
        "test_property_single",
        "test_properties_multiple",
     });
}

void test_property_bad_arguments () {
    expect_function("query_property", testOb);
    expect_function("query_properties", testOb);
    expect_function("set_property", testOb);
    expect_function("set_property", testOb);
    expect_function("set_properties", testOb);
    expect_function("add_property", testOb);
    expect_function("remove_property", testOb);

    expect("query_property handled invalid argument 1", (: ({
        assert((: testOb->query_property() :), "catch", "*Bad argument 1 to property->query_property\n"),
        assert((: testOb->query_property(0) :), "catch", "*Bad argument 1 to property->query_property\n"),
        assert((: testOb->query_property(0.0) :), "catch", "*Bad argument 1 to property->query_property\n"),
        assert((: testOb->query_property("") :), "catch", "*Bad argument 1 to property->query_property\n"),
        assert((: testOb->query_property(({})) :), "catch", "*Bad argument 1 to property->query_property\n"),
        assert((: testOb->query_property(([])) :), "catch", "*Bad argument 1 to property->query_property\n"),
    }) :));

    expect("set_properties handled invalid argument 1", (: ({
        assert((: testOb->set_properties() :), "catch", "*Bad argument 1 to property->set_properties\n"),
        assert((: testOb->set_properties(0) :), "catch", "*Bad argument 1 to property->set_properties\n"),
        assert((: testOb->set_properties(1.0) :), "catch", "*Bad argument 1 to property->set_properties\n"),
        assert((: testOb->set_properties("") :), "catch", "*Bad argument 1 to property->set_properties\n"),
        assert((: testOb->set_properties(({})) :), "catch", "*Bad argument 1 to property->set_properties\n"),
        assert((: testOb->set_properties(([])) :), "catch", "*Bad argument 1 to property->set_properties\n"),
    }) :));

    expect("set_property handled invalid argument 1", (: ({
        assert((: testOb->set_property() :), "catch", "*Bad argument 1 to property->set_property\n"),
        assert((: testOb->set_property(0, 0) :), "catch", "*Bad argument 1 to property->set_property\n"),
        assert((: testOb->set_property(0.0, 0.0) :), "catch", "*Bad argument 1 to property->set_property\n"),
        assert((: testOb->set_property("", "") :), "catch", "*Bad argument 1 to property->set_property\n"),
        assert((: testOb->set_property(({}), ({})) :), "catch", "*Bad argument 1 to property->set_property\n"),
        assert((: testOb->set_property(([]), ([])) :), "catch", "*Bad argument 1 to property->set_property\n"),
        assert((: testOb->set_property() :), "catch", "*Bad argument 1 to property->set_property\n"),
    }) :));
    expect("set_property handled invalid argument 2", (: ({
        assert((: testOb->set_property("test_key") :), "catch", "*Bad argument 2 to property->set_property\n"),
        assert((: testOb->set_property("test_key", "") :), "catch", "*Bad argument 2 to property->set_property\n"),
        assert((: testOb->set_property("test_key", ({})) :), "catch", "*Bad argument 2 to property->set_property\n"),
        assert((: testOb->set_property("test_key", ([])) :), "catch", "*Bad argument 2 to property->set_property\n"),
    }) :));

    expect("add_property handled invalid argument 1", (: ({
        assert((: testOb->add_property() :), "catch", "*Bad argument 1 to property->add_property\n"),
        assert((: testOb->add_property(0, 0) :), "catch", "*Bad argument 1 to property->add_property\n"),
        assert((: testOb->add_property(0.0, 0.0) :), "catch", "*Bad argument 1 to property->add_property\n"),
        assert((: testOb->add_property("", "") :), "catch", "*Bad argument 1 to property->add_property\n"),
        assert((: testOb->add_property(({}), ({})) :), "catch", "*Bad argument 1 to property->add_property\n"),
        assert((: testOb->add_property(([]), ([])) :), "catch", "*Bad argument 1 to property->add_property\n"),
    }) :));
    expect("add_property handled invalid argument 2", (: ({
        assert((: testOb->add_property("test_key") :), "catch", "*Bad argument 2 to property->add_property\n"),
        assert((: testOb->add_property("test_key", "") :), "catch", "*Bad argument 2 to property->add_property\n"),
        assert((: testOb->add_property("test_key", ({})) :), "catch", "*Bad argument 2 to property->add_property\n"),
        assert((: testOb->add_property("test_key", ([])) :), "catch", "*Bad argument 2 to property->add_property\n"),
    }) :));

    expect("remove_property handled invalid argument 1", (: ({
        assert((: testOb->remove_property() :), "catch", "*Bad argument 1 to property->remove_property\n"),
        assert((: testOb->remove_property(0) :), "catch", "*Bad argument 1 to property->remove_property\n"),
        assert((: testOb->remove_property(0.0) :), "catch", "*Bad argument 1 to property->remove_property\n"),
        assert((: testOb->remove_property("") :), "catch", "*Bad argument 1 to property->remove_property\n"),
        assert((: testOb->remove_property(({})) :), "catch", "*Bad argument 1 to property->remove_property\n"),
        assert((: testOb->remove_property(([])) :), "catch", "*Bad argument 1 to property->remove_property\n"),
    }) :));
}

void test_property_single () {
    expect("add, set, remove, and query property are handled", (: ({
        assert(testOb->query_property("test_key"), "==", UNDEFINED),

        assert(testOb->set_property("test_key1", "test_value"), "==", "test_value"),
        assert(testOb->query_property("test_key1"), "==", "test_value"),

        assert(testOb->set_property("test_key2", to_float(123)), "==", to_float(123)),
        assert(testOb->query_property("test_key2"), "==", to_float(123)),

        assert(testOb->add_property("test_key3", 10), "==", 10),
        assert(testOb->query_property("test_key3"), "==", 10),
        assert(testOb->add_property("test_key3", 1), "==", 11),
        assert(testOb->query_property("test_key3"), "==", 11),

        assert(testOb->remove_property("test_key1"), "==", 1),
        assert(testOb->query_property("test_key1"), "==", UNDEFINED),
        assert(testOb->remove_property("test_key2"), "==", 1),
        assert(testOb->query_property("test_key2"), "==", UNDEFINED),
    }) :));
}

private mapping tmp;
private mapping m1 = ([ "test_key1": "test_value" ]);
private mapping m2 = ([ "test_key2": to_float(123) ]);

void test_properties_multiple () {
    expect("set and query properties are handled", (: ({
        assert(testOb->query_properties(), "==", (: identify(([])) :)),
        assert(tmp = m1, "==", identify(m1)),
        assert(testOb->set_properties(m1), "==", identify(tmp)),
        assert(testOb->query_properties(), "==", identify(tmp)),
        assert(tmp = tmp + m2, "==", identify(m1 + m2)),
        assert(testOb->set_properties(m2), "==", identify(tmp)),
        assert(testOb->query_properties(), "==", identify(tmp)),
        assert(testOb->remove_property("test_key2"), "==", 1),
        assert(testOb->query_properties(), "==", identify(m1)),
    }) :));
}