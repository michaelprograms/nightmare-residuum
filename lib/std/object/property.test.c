inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/object/property.c");
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
    string *values, *results;

    expect_function("query_property", testOb);
    expect_function("query_properties", testOb);
    expect_function("set_property", testOb);
    expect_function("set_property", testOb);
    expect_function("set_properties", testOb);
    expect_function("add_property", testOb);
    expect_function("remove_property", testOb);

    values = ({});
    results = ({});
    values += ({ catch(testOb->query_property()) });
    results += ({ "*Bad argument 1 to property->query_property\n" });
    values += ({ catch(testOb->query_property(0)) });
    results += ({ "*Bad argument 1 to property->query_property\n" });
    values += ({ catch(testOb->query_property(0.0)) });
    results += ({ "*Bad argument 1 to property->query_property\n" });
    values += ({ catch(testOb->query_property("")) });
    results += ({ "*Bad argument 1 to property->query_property\n" });
    values += ({ catch(testOb->query_property(({}))) });
    results += ({ "*Bad argument 1 to property->query_property\n" });
    values += ({ catch(testOb->query_property(([]))) });
    results += ({ "*Bad argument 1 to property->query_property\n" });
    expect_arrays_equal(values, results, "query_property handled invalid inputs");

    values = ({});
    results = ({});
    values += ({ catch(testOb->set_properties()) });
    results += ({ "*Bad argument 1 to property->set_properties\n" });
    values += ({ catch(testOb->set_properties(0)) });
    results += ({ "*Bad argument 1 to property->set_properties\n" });
    values += ({ catch(testOb->set_properties(1.0)) });
    results += ({ "*Bad argument 1 to property->set_properties\n" });
    values += ({ catch(testOb->set_properties("")) });
    results += ({ "*Bad argument 1 to property->set_properties\n" });
    values += ({ catch(testOb->set_properties(({}))) });
    results += ({ "*Bad argument 1 to property->set_properties\n" });
    values += ({ catch(testOb->set_properties(([]))) });
    results += ({ "*Bad argument 1 to property->set_properties\n" });
    expect_arrays_equal(values, results, "set_properties handled invalid inputs");

    values = ({});
    results = ({});
    values += ({ catch(testOb->set_property()) });
    results += ({ "*Bad argument 1 to property->set_property\n" });
    values += ({ catch(testOb->set_property(0, 0)) });
    results += ({ "*Bad argument 1 to property->set_property\n" });
    values += ({ catch(testOb->set_property(0.0, 0.0)) });
    results += ({ "*Bad argument 1 to property->set_property\n" });
    values += ({ catch(testOb->set_property("", "")) });
    results += ({ "*Bad argument 1 to property->set_property\n" });
    values += ({ catch(testOb->set_property(({}), ({}))) });
    results += ({ "*Bad argument 1 to property->set_property\n" });
    values += ({ catch(testOb->set_property(([]), ([]))) });
    results += ({ "*Bad argument 1 to property->set_property\n" });
    values += ({ catch(testOb->set_property("test_key")) });
    results += ({ "*Bad argument 2 to property->set_property\n" });
    values += ({ catch(testOb->set_property("test_key", "")) });
    results += ({ "*Bad argument 2 to property->set_property\n" });
    values += ({ catch(testOb->set_property("test_key", ({}))) });
    results += ({ "*Bad argument 2 to property->set_property\n" });
    values += ({ catch(testOb->set_property("test_key", ([]))) });
    results += ({ "*Bad argument 2 to property->set_property\n" });
    expect_arrays_equal(values, results, "set_property handled invalid inputs");

    values = ({});
    results = ({});
    values += ({ catch(testOb->add_property()) });
    results += ({ "*Bad argument 1 to property->add_property\n" });
    values += ({ catch(testOb->add_property(0, 0)) });
    results += ({ "*Bad argument 1 to property->add_property\n" });
    values += ({ catch(testOb->add_property(0.0, 0.0)) });
    results += ({ "*Bad argument 1 to property->add_property\n" });
    values += ({ catch(testOb->add_property("", "")) });
    results += ({ "*Bad argument 1 to property->add_property\n" });
    values += ({ catch(testOb->add_property(({}), ({}))) });
    results += ({ "*Bad argument 1 to property->add_property\n" });
    values += ({ catch(testOb->add_property(([]), ([]))) });
    results += ({ "*Bad argument 1 to property->add_property\n" });
    values += ({ catch(testOb->add_property("test_key")) });
    results += ({ "*Bad argument 2 to property->add_property\n" });
    values += ({ catch(testOb->add_property("test_key", "")) });
    results += ({ "*Bad argument 2 to property->add_property\n" });
    values += ({ catch(testOb->add_property("test_key", ({}))) });
    results += ({ "*Bad argument 2 to property->add_property\n" });
    values += ({ catch(testOb->add_property("test_key", ([]))) });
    results += ({ "*Bad argument 2 to property->add_property\n" });
    expect_arrays_equal(values, results, "add_property handled invalid inputs");

    values = ({});
    results = ({});
    values += ({ catch(testOb->remove_property()) });
    results += ({ "*Bad argument 1 to property->remove_property\n" });
    values += ({ catch(testOb->remove_property(0)) });
    results += ({ "*Bad argument 1 to property->remove_property\n" });
    values += ({ catch(testOb->remove_property(0.0)) });
    results += ({ "*Bad argument 1 to property->remove_property\n" });
    values += ({ catch(testOb->remove_property("")) });
    results += ({ "*Bad argument 1 to property->remove_property\n" });
    values += ({ catch(testOb->remove_property(({}))) });
    results += ({ "*Bad argument 1 to property->remove_property\n" });
    values += ({ catch(testOb->remove_property(([]))) });
    results += ({ "*Bad argument 1 to property->remove_property\n" });
    expect_arrays_equal(values, results, "remove_property handled invalid inputs");
}

void test_property_single () {
    string *values = ({}), *results = ({});

    values += ({ testOb->query_property("test_key") });
    results += ({ UNDEFINED });

    testOb->set_property("test_key1", "test_value");
    values += ({ testOb->query_property("test_key1") });
    results += ({ "test_value" });

    testOb->set_property("test_key2", to_float(123));
    values += ({ testOb->query_property("test_key2") });
    results += ({ to_float(123) });

    testOb->add_property("test_key3", 10);
    values += ({ testOb->query_property("test_key3") });
    results += ({ 10 });
    testOb->add_property("test_key3", 1);
    values += ({ testOb->query_property("test_key3") });
    results += ({ 11 });

    testOb->remove_property("test_key1");
    values += ({ testOb->query_property("test_key1") });
    results += ({ UNDEFINED });
    testOb->remove_property("test_key2");
    values += ({ testOb->query_property("test_key2") });
    results += ({ UNDEFINED });

    expect_arrays_equal(values, results, "add, set, remove, and query property are handled");
}

void test_properties_multiple () {
    string *values = ({}), *results = ({});
    mapping tmp;
    mapping m1 = ([ "test_key1": "test_value" ]);
    mapping m2 = ([ "test_key2": to_float(123) ]);

    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(([])) });

    tmp = m1;
    testOb->set_properties(m1);
    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(tmp) });

    tmp += m2;
    testOb->set_properties(m2);
    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(tmp) });

    testOb->remove_property("test_key2");
    values += ({ identify(testOb->query_properties()) });
    results += ({ identify(tmp) });

    expect_arrays_equal(values, results, "set and query properties are handled");
}