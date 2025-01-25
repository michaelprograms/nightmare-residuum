inherit M_TEST;
inherit STD_STORAGE;
inherit "/std/living/vitals";

/**
 * @var {"/std/consumable"} testOb
 */

void test_is_consumable () {
    expect("is_consumable returns true", (: ({
        assert_equal(itemp(testOb), 1),
        assert_equal(testOb->is_consumable(), 1),
        assert_equal(drinkp(testOb), 0),
        assert_equal(foodp(testOb), 0),
    }) :));

    expect("use verb is correct", (: ({
        assert_equal(testOb->query_use_verb(), "consume"),
    }) :));
}

void test_strength () {
    expect("consumable handles strength", (: ({
        assert_equal(testOb->query_strength(), 0),
        testOb->set_strength(5),
        assert_equal(testOb->query_strength(), 5),
        testOb->set_strength(100),
        assert_equal(testOb->query_strength(), 100),
        testOb->set_strength(0),
        assert_equal(testOb->query_strength(), 0),
        testOb->set_strength(-5),
        assert_equal(testOb->query_strength(), -5),
    }) :));
}

void test_handle_consume () {
    object r = new(STD_ROOM);

    expect("consumable handle_consume behaves", (: ({
        // setup consumable
        testOb->set_strength(5),
        assert_equal(testOb->query_strength(), 5),

        // setup test object
        assert_equal(this_object()->handle_move($(r)), 1),
        assert_equal(this_object()->update_vitals(), 0), // initialize vitals
        assert_equal(this_object()->add_hp(5), 0),
        assert_equal(this_object()->query_hp(), 5),
        assert_equal(this_object()->add_sp(5), 0),
        assert_equal(this_object()->query_sp(), 5),
        assert_equal(this_object()->add_mp(5), 0),
        assert_equal(this_object()->query_mp(), 5),

        // test consuming
        assert_equal(testOb->handle_consume(this_object()), 0),
        assert_equal(this_object()->query_hp(), 10),
        assert_equal(this_object()->query_sp(), 10),
        assert_equal(this_object()->query_mp(), 10),

        assert_equal(objectp(testOb), 0), // consumable was removed
    }) :));

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}