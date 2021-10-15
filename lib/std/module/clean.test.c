inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/clean.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_clean_up () {
    int *values = ({}), *results = ({});

    expect_function("query_no_clean", testOb);
    expect_function("set_no_clean", testOb);
    expect_function("clean_up", testOb);
    expect_function("clean_never", testOb);
    expect_function("clean_later", testOb);

    values += ({ testOb->clean_never() });
    results += ({ 0 });
    values += ({ testOb->clean_later() });
    results += ({ 1 });

    values += ({ testOb->query_no_clean() });
    results += ({ 0 });
    testOb->set_no_clean(1);
    values += ({ testOb->query_no_clean() });
    results += ({ 1 });
    values += ({ testOb->clean_up() });
    results += ({ 0 });
    testOb->set_no_clean(0);
    values += ({ testOb->query_no_clean() });
    results += ({ 0 }); // clean_later()
    values += ({ objectp(testOb) });
    results += ({ 1 });
    values += ({ testOb->clean_up() });
    results += ({ 1 });
    values += ({ objectp(testOb) });
    results += ({ 1 }); // @TODO should be 0? object is not destructing because children > 1

    // @TODO check contents moved after internal_remove ?

    expect_arrays_equal(values, results, "clean_up behaves");
}