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

void test_query_parent () {
    expect("query_parent returns this_object", (: ({
        assert_equal(testOb->query_parent(), this_object()),
    }) :));
}

void test_set_parent () {
    object ob = new ("/std/object.c");

    expect("set_parent returns new object", (: ({
        // invalid object
        assert_equal(testOb->set_parent($(ob)), 0),
        // valid object (for testing)
        assert_equal(testOb->set_parent(this_object()), 1),
    }) :));

    destruct(ob);
}