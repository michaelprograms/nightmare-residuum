inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

void test_distinct_array () {
    expect("distinct_array handles inputs", (: ({
        assert_equal(testOb->distinct_array(({ 1, 2, 2, 3, 2, 1, 3, 2, 1 })), ({ 3, 2, 1 })),
        assert_equal(testOb->distinct_array(({ 1, 4, 4, 3, 4, 1, 3, 2, 1 })), ({ 4, 3, 2, 1 })),
    }) :));
}

void test_reduce () {
    expect("reduce handles array", (: ({
        assert_equal(testOb->reduce(({ 1, 2, 3, 4, 5 }), (: $1 + $2 :), 0), 15),
        assert_equal(testOb->reduce(({ 1, 3, 5, 7, 9, }), (: $1 + $2 :), 0), 25),
    }) :));
}