inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item/key.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_is_key () {
    expect_function("is_key", testOb);

    expect("is_key returns true", (: ({
        assert_equal(testOb->is_key(), 1),
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_weapon(), 0),
        assert_equal(testOb->is_armor(), 0),
        assert_equal(testOb->is_living(), 0),
    }) :));
}

void test_direct_unlock_str_with_obj () {
    expect_function("direct_unlock_str_with_obj", testOb);

    expect("direct_unlock_str_with_obj returns true", (: ({
        assert_equal(testOb->direct_unlock_str_with_obj(), 0),
        assert_equal(testOb->direct_unlock_str_with_obj("direction"), 0),
        assert_equal(testOb->direct_unlock_str_with_obj("direction", "bad key"), 0),
        assert_equal(testOb->direct_unlock_str_with_obj("direction", testOb), 1),
    }) :));
}