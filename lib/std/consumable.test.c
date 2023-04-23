inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/consumable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_is_consumable", "test_strength", "test_handle_consume", });
}

void test_is_consumable () {
    expect_function("is_consumable", testOb);
    expect_function("is_drink", testOb);
    expect_function("is_food", testOb);

    expect("is_consumable returns true", (: ({
        assert(testOb->is_consumable(), "==", 1),
        assert(testOb->is_drink(), "==", 0),
        assert(testOb->is_food(), "==", 0),
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

void test_handle_consume () {
    object r = new(STD_ROOM);

    expect_function("handle_consume", testOb);

    expect("food handle_consume behaves", (: ({
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
        assert(testOb->handle_consume(this_object()), "==", 0),
        assert(this_object()->query_hp(), "==", 10),
        assert(this_object()->query_sp(), "==", 10),
        assert(this_object()->query_mp(), "==", 10),

        assert(objectp(testOb), "==", 0), // food was removed
    }) :));

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}