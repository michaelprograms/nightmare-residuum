inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/array");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_distinct_array () {
    int *arr1 = ({1, 2, 2, 3, 2, 1, 3, 2, 1});

    expect_function("distinct_array", testOb);
    expect_arrays_equal(testOb->distinct_array(arr1), ({3, 2, 1}), "distinct array returned distinct members");
    expect_arrays_equal(testOb->distinct_array(({})), ({}), "distinct array handled empty array");
    expect_arrays_equal(testOb->distinct_array(0), ({}), "distinct array handled blank parameter");
}