inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_living () {
    expect_function("is_living", testOb);

    expect_true(testOb->is_living(), "is_living handled");
}

void test_species () {
    mixed *values = ({}), *results = ({});

    expect_function("query_species", testOb);
    expect_function("set_species", testOb);

    values += ({ testOb->query_species() });
    results += ({ "unknown" });

    testOb->set_species("human");
    values += ({ testOb->query_species() });
    results += ({ "human" });

    expect_arrays_equal(values, results, "living handled species");
}