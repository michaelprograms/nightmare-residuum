inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

/**
 * @var {"/std/item/drink"} testOb
 */

string *test_order () {
    return ({ "test_is_drink", "test_item_verb_drink_applies", });
}

void test_is_drink () {
    expect("is_consumable returns true", (: ({
        assert_equal(testOb->is_consumable(), 1),
        assert_equal(drinkp(testOb), 1),
        assert_equal(foodp(testOb), 0),
        assert_equal(itemp(testOb), 1),
        assert_equal(weaponp(testOb), 0),
    }) :));

    expect("use verb is correct", (: ({
        assert_equal(testOb->query_use_verb(), "drink"),
    }) :));
}

void test_item_verb_drink_applies () {
    expect("drink handles verb apply direct_drink_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_drink_obj(), 0),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_drink_obj(), 1),
    }) :));
}