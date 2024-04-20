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

void test_access () {
    expect("unguarded evaluates function", (: ({
        assert_equal(testOb->unguarded((: MAX_INT :)), MAX_INT),
        assert_equal(testOb->unguarded(function () { return MAX_INT; }), MAX_INT),
    }) :));
}