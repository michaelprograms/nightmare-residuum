inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/object/description");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_long () {
    int *values = ({}), *results = ({});

    expect_function("set_long", testOb);
    expect_function("query_long", testOb);

    values += ({ testOb->query_long() });
    results += ({ UNDEFINED });

    testOb->set_long("Test long");
    values += ({ testOb->query_long() });
    results += ({ "Test long" });

    testOb->set_long((: base_name(previous_object(1)) :));
    values += ({ testOb->query_long() });
    results += ({ base_name(this_object()) });

    testOb->set_long(0);
    values += ({ testOb->query_long() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "long descriptions are handled");
}

void test_short () {
    int *values = ({}), *results = ({});

    expect_function("set_short", testOb);
    expect_function("query_short", testOb);

    values += ({ testOb->query_short() });
    results += ({ UNDEFINED });

    testOb->set_short("Test short");
    values += ({ testOb->query_short() });
    results += ({ "Test short" });

    testOb->set_short((: base_name(previous_object(1)) :));
    values += ({ testOb->query_short() });
    results += ({ base_name(this_object()) });

    testOb->set_short(0);
    values += ({ testOb->query_short() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "short descriptions are handled");
}