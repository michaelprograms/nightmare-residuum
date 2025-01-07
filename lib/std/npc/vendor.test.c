inherit M_TEST;
inherit M_MOVE;

/**
 * @var {"/std/npc/vendor"} testOb
 */

void after_each_test () {
    // vendor handle_remove to destruct vendor inventory
    if (testOb) testOb->handle_remove();
}

void test_vendor () {
    expect("is_vendor behaves", (: ({
        assert_equal(testOb->is_living(), 1),
        assert_equal(testOb->is_vendor(), 1),
        assert_equal(testOb->is_npc(), 1),
    }) :));
}

void test_vendor_inventory () {
    object vi, ob;

    // grab reference to vendor inventory
    vi = testOb->query_vendor_inventory();
    // setup a test item
    ob = new(STD_ITEM);

    expect("vendor inventory is created and cleaned", (: ({
        // vendor inventory exists
        assert_equal(objectp($(vi)), 1),
        // move test item to vendor inventory
        assert_equal($(ob)->handle_move($(vi)), 1),
        // remove vendor
        assert_equal(testOb->handle_remove(), 1),
        // verify vendor inventory is destructed
        assert_equal(objectp($(vi)), 0),
        // verify item is destructed
        assert_equal(objectp($(ob)), 0),
    }) :));

    if (ob) destruct(ob);
}

void test_max_items () {
    expect("vendor max items behaves", (: ({
        // verify default max items
        assert_equal(testOb->query_vendor_max_items(), 0),
        // verify changing max items
        testOb->set_vendor_max_items(10),
        assert_equal(testOb->query_vendor_max_items(), 10),
        // verify changing max items
        testOb->set_vendor_max_items(123),
        assert_equal(testOb->query_vendor_max_items(), 123),
    }) :));
}

void test_vendor_currency () {
    expect("vendor currency is settable and queryable", (: ({
        assert_equal(testOb->query_vendor_currency(), 0),

        testOb->set_vendor_currency("copper"),
        assert_equal(testOb->query_vendor_currency(), "copper"),

        testOb->set_vendor_currency("silver"),
        assert_equal(testOb->query_vendor_currency(), "silver"),
    }) :));
}

void test_vendor_type () {
    expect("vendor type is settable and queryable", (: ({
        assert_equal(testOb->query_vendor_types(), 0),

        testOb->set_vendor_types(({ STD_WEAPON })),
        assert_equal(testOb->query_vendor_types(), ({ STD_WEAPON })),

        testOb->set_vendor_types(({ STD_ARMOR })),
        assert_equal(testOb->query_vendor_types(), ({ STD_ARMOR })),

        testOb->set_vendor_types(({ STD_ITEM })),
        assert_equal(testOb->query_vendor_types(), ({ STD_ITEM })),

        testOb->set_vendor_types(({ STD_ARMOR, STD_WEAPON })),
        assert_equal(testOb->query_vendor_types(), ({ STD_ARMOR, STD_WEAPON })),
    }) :));
}

void test_apply_list_verb () {
    object room;

    room = new(STD_ROOM);

    this_object()->handle_move("/domain/Nowhere/room/void.c");

    expect("direct_list_from_obj returns true when same environment", (: ({
        // true with same environment
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(this_object()->query_environment_path(), "/domain/Nowhere/room/void.c"),
        assert_equal(testOb->direct_list_from_obj(testOb), 1),
        // false with no object sent
        assert_equal(testOb->direct_list_from_obj(), 0),
        // false with different environments
        assert_equal(testOb->handle_move($(room)), 1),
        assert_equal(testOb->direct_list_from_obj(testOb), 0),
    }) :));

    expect("direct_list_str_from_obj returns true when same environment", (: ({
        // true with same environment
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(this_object()->query_environment_path(), "/domain/Nowhere/room/void.c"),
        assert_equal(testOb->direct_list_from_obj(testOb), 1),
        // false with no object sent
        assert_equal(testOb->direct_list_from_obj(), 0),
        // false with different environments
        assert_equal(testOb->handle_move($(room)), 1),
        assert_equal(testOb->direct_list_from_obj(testOb), 0),
    }) :));

    testOb->handle_remove();
    room->handle_remove();
}

void test_apply_buy_verb () {
    object room;

    room = new(STD_ROOM);

    this_object()->handle_move("/domain/Nowhere/room/void.c");
    expect("direct_buy_str_from_obj returns true when same environment", (: ({
        // true with same environment
        assert_equal(testOb->handle_move("/domain/Nowhere/room/void.c"), 1),
        assert_equal(this_object()->query_environment_path(), "/domain/Nowhere/room/void.c"),
        assert_equal(testOb->direct_buy_str_from_obj("", testOb), 1),
        // false with no object sent
        assert_equal(testOb->direct_buy_str_from_obj(""), 0),
        assert_equal(testOb->direct_buy_str_from_obj(), 0),
        // false with different environments
        assert_equal(testOb->handle_move($(room)), 1),
        assert_equal(testOb->direct_buy_str_from_obj("", testOb), 0),
    }) :));

    testOb->handle_remove();
    room->handle_remove();
}