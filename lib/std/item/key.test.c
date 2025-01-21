inherit M_TEST;

/**
 * @var {"/std/item/key"} testOb
 */

void test_is_key () {
    expect("is_key returns true", (: ({
        assert_equal(testOb->is_key(), 1),
        assert_equal(itemp(testOb), 1),
        assert_equal(weaponp(testOb), 0),
        assert_equal(armorp(testOb), 0),
        assert_equal(livingp(testOb), 0),
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