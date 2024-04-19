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

void test_level () {
    expect_function("set_level", testOb);
    expect_function("query_level", testOb);

    expect("level settable and queryable", (: ({
        assert_equal(testOb->query_level(), 1),

        testOb->set_level(123),
        assert_equal(testOb->query_level(), 123),

        testOb->set_level(0),
        assert_equal(testOb->query_level(), 0),

        testOb->set_level(-123),
        assert_equal(testOb->query_level(), 123),
    }) :));
}