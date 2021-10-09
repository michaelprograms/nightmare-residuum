inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living/stats");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_and_set_stat () {
    int *values = ({}), *results = ({});

    expect_function("query_stat", testOb);
    expect_function("set_stat", testOb);

    testOb->set_stat("strength", 12345);
    testOb->set_stat("perception", 12345);
    testOb->set_stat("endurance", 12345);
    testOb->set_stat("charisma", 12345);
    testOb->set_stat("intelligence", 12345);
    testOb->set_stat("agility", 12345);
    testOb->set_stat("luck", 12345);

    values += ({ testOb->query_stat("strength") });
    results += ({ 12345 });
    values += ({ testOb->query_stat("perception") });
    results += ({ 12345 });
    values += ({ testOb->query_stat("endurance") });
    results += ({ 12345 });
    values += ({ testOb->query_stat("charisma") });
    results += ({ 12345 });
    values += ({ testOb->query_stat("intelligence") });
    results += ({ 12345 });
    values += ({ testOb->query_stat("agility") });
    results += ({ 12345 });
    values += ({ testOb->query_stat("luck") });
    results += ({ 12345 });

    expect_arrays_equal (values, results, "set_stat and query_stat handled numbers");
}