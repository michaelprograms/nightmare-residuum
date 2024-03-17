inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item/drink.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_is_drink", "test_item_verb_drink_applies", });
}

void test_is_drink () {
    expect_function("is_drink", testOb);

    expect("is_consumable returns true", (: ({
        assert_equal(testOb->is_consumable(), 1),
        assert_equal(testOb->is_drink(), 1),
        assert_equal(testOb->is_food(), 0),
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_weapon(), 0),
    }) :));
}

void test_item_verb_drink_applies () {
    expect_function("direct_drink_obj", testOb);

    expect("drink handles verb apply direct_drink_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_drink_obj(), 0),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_drink_obj(), 1),
    }) :));
}