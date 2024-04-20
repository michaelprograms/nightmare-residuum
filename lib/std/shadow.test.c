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

void test_shadow () {
    expect("shadow handled starting and stopping", (: ({
        assert_equal(testOb->query_shadow(), UNDEFINED),
        assert_equal(testOb->start_shadow(this_object()), 1),
        assert_equal(testOb->query_shadow(), this_object()),
        assert_equal(testOb->stop_shadow(), 1),
    }) :));
}