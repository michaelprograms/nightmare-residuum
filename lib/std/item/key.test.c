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

void test_is_key () {
    expect("is_key returns true", (: ({
        assert_equal(testOb->is_key(), 1),
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_weapon(), 0),
        assert_equal(testOb->is_armor(), 0),
        assert_equal(testOb->is_living(), 0),
    }) :));
}

void test_direct_lock_str_with_obj () {
    expect("direct_lock_str_with_obj returns true", (: ({
        assert_equal(testOb->direct_lock_str_with_obj(), 0),
        assert_equal(testOb->direct_lock_str_with_obj("direction"), 0),
        assert_equal(testOb->direct_lock_str_with_obj("direction", "bad key"), 0),
        assert_equal(testOb->direct_lock_str_with_obj("direction", testOb), 1),
    }) :));
}
void test_direct_unlock_str_with_obj () {
    expect("direct_unlock_str_with_obj returns true", (: ({
        assert_equal(testOb->direct_unlock_str_with_obj(), 0),
        assert_equal(testOb->direct_unlock_str_with_obj("direction"), 0),
        assert_equal(testOb->direct_unlock_str_with_obj("direction", "bad key"), 0),
        assert_equal(testOb->direct_unlock_str_with_obj("direction", testOb), 1),
    }) :));
}