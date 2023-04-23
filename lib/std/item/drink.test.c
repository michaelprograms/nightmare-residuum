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
    return ({ "test_is_drink", "test_strength", "test_item_verb_drink_applies", "test_handle_drink", });
}

void test_is_drink () {
    expect_function("is_drink", testOb);
    expect("is_drink returns true", (: ({
        assert(testOb->is_drink(), "==", 1),
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_weapon(), "==", 0),
    }) :));
}

void test_strength () {
    expect_function("query_strength", testOb);
    expect_function("set_strength", testOb);

    expect("drink handles strength", (: ({
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

void test_item_verb_drink_applies () {
    expect_function("direct_drink_obj", testOb);

    expect("drink handles verb apply direct_drink_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(testOb->direct_drink_obj(), "==", 0),
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(environment(testOb), "==", this_object()),
        assert(testOb->direct_drink_obj(), "==", 1),
    }) :));
}

void test_handle_drink () {
    object r = new(STD_ROOM);

    expect_function("handle_drink", testOb);

    expect("drink handle_drink behaves", (: ({
        // setup drink
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

        // test drinking
        assert(testOb->handle_drink(this_object()), "==", 0),
        assert(this_object()->query_hp(), "==", 10),
        assert(this_object()->query_sp(), "==", 10),
        assert(this_object()->query_mp(), "==", 10),

        assert(objectp(testOb), "==", 0), // drink was removed
    }) :));

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}