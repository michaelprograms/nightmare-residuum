inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living/body.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_gender () {
    string *values = ({}), *results = ({});

    expect_function("set_gender", testOb);
    expect_function("query_gender", testOb);

    values += ({ testOb->query_gender() });
    results += ({ "none" });
    testOb->set_gender("male");
    values += ({ testOb->query_gender() });
    results += ({ "male" });
    testOb->set_gender("female");
    values += ({ testOb->query_gender() });
    results += ({ "female" });
    testOb->set_gender("neither");
    values += ({ testOb->query_gender() });
    results += ({ "neither" });
    testOb->set_gender(0);
    values += ({ testOb->query_gender() });
    results += ({ "none" });
    testOb->set_gender("invalid");
    values += ({ testOb->query_gender() });
    results += ({ "none" });

    expect_arrays_equal(values, results, "gender handled");
}

void test_level () {
    int *values = ({}), *results = ({});

    expect_function("set_level", testOb);
    expect_function("query_level", testOb);

    values += ({ intp(testOb->query_level()) });
    results += ({ 1 });
    values += ({ testOb->query_level() });
    results += ({ 0 });

    testOb->set_level(123);
    values += ({ testOb->query_level() });
    results += ({ 123 });

    expect_arrays_equal(values, results, "level handled");
}