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

void test_lifecycle_functions () {
    // @TODO test these better
}

void test_parser_applies () {
    expect("object handles parser applies", (: ({
        assert_equal(testOb->is_living(), 0),
        assert_equal(testOb->inventory_visible(), 0),
        assert_equal(testOb->inventory_accessible(), 0),
    }) :));
}