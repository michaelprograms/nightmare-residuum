inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living/body");
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