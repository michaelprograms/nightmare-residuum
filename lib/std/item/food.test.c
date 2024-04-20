inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

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
string *test_order () {
    return ({ "test_is_food", "test_item_verb_eat_applies", });
}

void test_is_food () {
    expect("is_consumable returns true", (: ({
        assert_equal(testOb->is_consumable(), 1),
        assert_equal(testOb->is_drink(), 0),
        assert_equal(testOb->is_food(), 1),
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_weapon(), 0),
    }) :));
}

void test_item_verb_eat_applies () {
    expect("food handles verb apply direct_eat_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_eat_obj(), 0),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_eat_obj(), 1),
    }) :));
}