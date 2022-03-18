inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/array");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_distinct_array () {
    expect_function("distinct_array", testOb);

    expect("distinct_array handles inputs", (: ({
        assert((: testOb->distinct_array(({ 1, 2, 2, 3, 2, 1, 3, 2, 1 })) :), "==", ({ 3, 2, 1 })),
        assert((: testOb->distinct_array(({ 1, 4, 4, 3, 4, 1, 3, 2, 1 })) :), "==", ({ 4, 3, 2, 1 })),
        assert((: testOb->distinct_array(({})) :), "==", ({})),
        assert((: testOb->distinct_array(0) :), "==", ({})),
    }) :));
}