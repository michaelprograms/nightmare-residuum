inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item/food.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_is_food", "test_item_verb_eat_applies", });
}

void test_is_food () {
    expect_function("is_food", testOb);

    expect("is_consumable returns true", (: ({
        assert(testOb->is_consumable(), "==", 1),
        assert(testOb->is_drink(), "==", 0),
        assert(testOb->is_food(), "==", 1),
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_weapon(), "==", 0),
    }) :));
}

void test_item_verb_eat_applies () {
    expect_function("direct_eat_obj", testOb);

    expect("food handles verb apply direct_eat_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(testOb->direct_eat_obj(), "==", 0),
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(environment(testOb), "==", this_object()),
        assert(testOb->direct_eat_obj(), "==", 1),
    }) :));
}