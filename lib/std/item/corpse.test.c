inherit M_TEST;

/**
 * @var {"/std/item/corpse"} testOb
 */

private mixed *calloutInfo;
void test_received () {
    expect("handle_received sets expire timer", (: ({
        assert_regex(file_name(testOb), "/std/item/corpse.coverage#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test corpse
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 1),
        assert_equal(sizeof(calloutInfo[0]), 3),
        assert_equal(calloutInfo[0][0], testOb),
    }) :));
}

void test_corpse () {
    expect("is_corpse behaves", (: ({
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_corpse(), 1),
        assert_equal(testOb->is_character(), UNDEFINED),
        assert_equal(testOb->is_npc(), UNDEFINED),
    }) :));
}

void test_body () {
    object liv = new(STD_LIVING);
    object item;

    liv->set_name("tester");
    liv->set_id(({ "tester" }));
    liv->set_short("a tester");

    expect("body is setup to match", (: ({
        testOb->setup_body($(liv)),
        // verify we set corpse information
        assert_equal(testOb->query_name(), "corpse of tester"),
        assert_equal(testOb->query_id(), ({ "corpse", "corpseoftester", "corpse of a tester", "corpse of tester", })),
        assert_equal(testOb->query_short(), "corpse of a tester"),
    }) :));
    expect("body handles bad argument to setup", (: ({
        assert_catch((: testOb->setup_body(0) :), "*Bad argument 1 to corpse->setup_body\n"),
    }) :));

    liv->set_short("a %^BOLD%^tester%^DEFAULT%^");
    expect("body handles short with DEFAULT", (: ({
        testOb->setup_body($(liv)),
        // verify %^DEFAULT%^ turns to %^RESET%^
        assert_equal(testOb->query_short(), "corpse of a %^BOLD%^tester%^RESET%^"),
        // verify we keep %^DEFAULT%^ instead of replacing to %^RESET%^
        assert_equal(testOb->query_short("%^DEFAULT%^"), "corpse of a %^BOLD%^tester%^DEFAULT%^"),
        // verify we override %^DEFAULT%^ with %^RESET%^
        assert_equal(testOb->query_short("%^RED%^"), "corpse of a %^BOLD%^tester%^RED%^"),
    }) :));

    liv->add_currency("copper", 12345);
    expect("body transfers currency", (: ({
        // verify no coins on corpse
        assert_equal(!!present("coins", testOb), 0),
        assert_equal($(liv)->query_currency("copper"), 12345),

        testOb->setup_body($(liv)),

        // verify coins moved to corpse
        assert_equal(!!present("coins", testOb), 1),
        assert_equal(present("coins", testOb)->query_currency("copper"), 12345),
        assert_equal($(liv)->query_currency("copper"), 0),
    }) :));

    liv->add_currency("copper", -12345);
    item = new(STD_ITEM);
    item->set_id(({ "item" }));
    item->set_name("item");
    item->set_short("item");
    item->handle_move(liv);
    expect("body transfers items", (: ({
        // verify no item on corpse
        assert_equal(!!present("item", testOb), 0),
        assert_equal(!!present("item", $(liv)), 1),

        testOb->setup_body($(liv)),

        // verify item moved to corpse
        assert_equal(!!present("item", testOb), 1),
        assert_equal(!!present("item", $(liv)), 0),
        assert_equal($(liv)->query_currency("copper"), 0),
    }) :));

    if (liv) destruct(liv);
    if (item) destruct(item);
}