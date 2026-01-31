inherit M_TEST;
inherit M_MOVE;
inherit M_CURRENCY;

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

        assert_catch((: testOb->set_vendor_currency(UNDEFINED) :), "*Bad argument 1 to vendor->set_vendor_currency\n"),
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

        assert_catch((: testOb->set_vendor_types(UNDEFINED) :), "*Bad argument 1 to vendor->set_vendor_types\n"),
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
        assert_equal(testOb->direct_list_str_from_obj("", testOb), 1),
        // false with no object sent
        assert_equal(testOb->direct_list_str_from_obj(), 0),
        // false with different environments
        assert_equal(testOb->handle_move($(room)), 1),
        assert_equal(testOb->direct_list_str_from_obj("", testOb), 0),
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

void test_list () {
    object r = new(STD_ROOM);
    object mockC1 = new("/std/npc.mock.c"); // TODO: this is weird, its not an NPC but this is the functionality we need
    object c1 = new(STD_CHARACTER);

    c1->set_name("testcharacter");
    mockC1->start_shadow(c1);
    c1->handle_move(r);
    testOb->set_name("test vendor");
    testOb->handle_move(r);

    expect("vendor handles listing inventory", (: ({
        // nothing to list
        testOb->handle_list(0, $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "say", "Test vendor says: I don't have any items for sale right now, Testcharacter." })),

        // non-existent item
        testOb->handle_list("test", $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "say", "Test vendor says: I don't have any 'test' for sale." })),

        // one item in list
        testOb->set_vendor_max_items(1),
        testOb->set_vendor_currency("copper"),
        testOb->query_vendor_inventory()->set_reset(([
            "/std/item/food.c": 1,
        ])),
        testOb->query_vendor_inventory()->query_item_contents()[0]->set_name("test food"),
        testOb->query_vendor_inventory()->query_item_contents()[0]->set_short("test food"),
        testOb->query_vendor_inventory()->query_item_contents()[0]->set_value(10),
        // full list
        testOb->handle_list(0, $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<2..<1], ({ ({ "say", "Test vendor says: I have the following items, Testcharacter." }), ({ "action", "  test food                     10 copper" }) })),
        // list item
        testOb->handle_list("test food", $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<2..<1], ({ ({ "say", "Test vendor says: I have the following 'test food' items, Testcharacter." }), ({ "action", "  test food                     10 copper" }) })),
    }) :));

    mockC1->stop_shadow();
    if (mockC1) destruct(mockC1);
    if (c1) destruct(c1);
    if (r) destruct(r);
}

void test_buy () {
    object r = new(STD_ROOM);
    object mockC1 = new("/std/npc.mock.c"); // TODO: this is weird, its not an NPC but this is the functionality we need
    object c1 = new(STD_CHARACTER);

    c1->set_name("testcharacter");
    mockC1->start_shadow(c1);
    c1->handle_move(r);
    testOb->set_name("test vendor");
    testOb->handle_move(r);

    expect("vendor handles buying items", (: ({
        // nothing to buy
        testOb->handle_buy(0, $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "say", "Test vendor says: I don't have any '0' for sale." })),
    }) :));

    mockC1->stop_shadow();
    if (mockC1) destruct(mockC1);
    if (c1) destruct(c1);
    if (r) destruct(r);
}

void test_sell () {
    object r = new(STD_ROOM);
    object mockC1 = new("/std/npc.mock.c"); // TODO: this is weird, its not an NPC but this is the functionality we need
    object c1 = new(STD_CHARACTER);
    object ob = new(STD_FOOD);

    c1->set_name("testcharacter");
    mockC1->start_shadow(c1);
    c1->handle_move(r);
    testOb->set_name("test vendor");
    testOb->handle_move(r);

    ob->set_name("junk food");
    ob->set_short("junk food");
    ob->set_value(10);

    expect("vendor handles selling items", (: ({
        // nothing to sell
        testOb->handle_sell(0, $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "say", "Test vendor says: You don't have an item to sell." })),

        testOb->handle_sell($(ob), $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "say", "Test vendor says: I don't buy food items." })),

        testOb->set_vendor_types(({ STD_FOOD })),
        assert_equal(testOb->query_vendor_types(), ({ STD_FOOD })),
        testOb->handle_sell($(ob), $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "say", "Test vendor says: My shop is full, I can't buy any more items." })),

        testOb->set_vendor_max_items(1),
        testOb->set_vendor_currency("copper"),
        testOb->handle_sell($(ob), $(c1)),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "action", "You sell junk food for 5 copper." })),
        assert_equal($(c1)->query_currency("copper"), 5),
    }) :));

    if (ob) destruct(ob);
    mockC1->stop_shadow();
    if (mockC1) destruct(mockC1);
    if (c1) destruct(c1);
    if (r) destruct(r);
}