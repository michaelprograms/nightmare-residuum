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

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("type is settable and queryable", (: ({
        assert_equal(testOb->query_type(), UNDEFINED),

        testOb->set_type("test"),
        assert_equal(testOb->query_type(), "test"),

        testOb->set_type("test2"),
        assert_equal(testOb->query_type(), "test2"),
    }) :));
}