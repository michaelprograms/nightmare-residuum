inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/food.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_is_food", "test_strength", "test_item_verb_eat_applies", "test_handle_eat", });
}

void test_is_food () {
    expect_function("is_food", testOb);
    expect("is_food returns true", (: ({
        assert(testOb->is_food(), "==", 1),
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_weapon(), "==", 0),
    }) :));
}

void test_strength () {
    expect_function("query_strength", testOb);
    expect_function("set_strength", testOb);

    expect("food handles strength", (: ({
        assert(testOb->query_strength(), "==", 0),
        testOb->set_strength(5),
        assert(testOb->query_strength(), "==", 5),
        testOb->set_strength(100),
        assert(testOb->query_strength(), "==", 100),
        testOb->set_strength(0),
        assert(testOb->query_strength(), "==", 0),
        testOb->set_strength(-5),
        assert(testOb->query_strength(), "==", -5),
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

void test_handle_eat () {
    object r = new(STD_ROOM);

    expect_function("handle_eat", testOb);

    expect("food handle_eat behaves", (: ({
        // setup food
        testOb->set_strength(5),
        assert(testOb->query_strength(), "==", 5),

        // setup test object
        assert(this_object()->handle_move($(r)), "==", 1),
        assert(this_object()->update_vitals(), "==", 0), // initialize vitals
        assert(this_object()->add_hp(5), "==", 0),
        assert(this_object()->query_hp(), "==", 5),
        assert(this_object()->add_sp(5), "==", 0),
        assert(this_object()->query_sp(), "==", 5),
        assert(this_object()->add_mp(5), "==", 0),
        assert(this_object()->query_mp(), "==", 5),

        // test eating
        assert(testOb->handle_eat(this_object()), "==", 0),
        assert(this_object()->query_hp(), "==", 10),
        assert(this_object()->query_sp(), "==", 10),
        assert(this_object()->query_mp(), "==", 10),

        assert(objectp(testOb), "==", 0), // food was removed
    }) :));

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}