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

void test_disabled_efuns () {
    expect("input_to throws error", (: ({
        assert_catch((: input_to() :), "*efun::input_to disabled\n"),
    }) :));
    expect("this_player throws error", (: ({
        assert_catch((: this_player() :), "*efun::this_player disabled\n"),
    }) :));
}

void test_efuns () {
    expect("users returns an array", (: ({
        assert_equal(arrayp(testOb->users()), 1),
    }) :));
}