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

void test_achievements () {
    expect("achievements are settable and queryable", (: ({
        assert_equal(testOb->query_achievements(), ({ })),

        assert_equal(testOb->set_achievement("test1"), 1),
        assert_equal(testOb->query_achievement("test1"), 1),
        assert_equal(testOb->set_achievement("test1"), 0), // already set
        assert_equal(testOb->query_achievement("test1"), 1),

        assert_equal(testOb->set_achievement("test2"), 1),
        assert_equal(testOb->query_achievement("test2"), 1),
        assert_equal(testOb->set_achievement("test2"), 0), // already set
        assert_equal(testOb->query_achievement("test2"), 1),

        assert_equal(testOb->query_achievements(), ({ "test2", "test1", })),

        testOb->remove_achievement("test2"),
        assert_equal(testOb->query_achievements(), ({ "test1", })),
    }) :));
}