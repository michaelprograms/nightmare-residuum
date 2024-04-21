inherit M_TEST;
inherit M_CURRENCY;
inherit M_CONTAINER;

void test_currency () {
    expect("is_currency behaves", (: ({
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_currency(), 1),
        assert_equal(testOb->is_character(), UNDEFINED),
        assert_equal(testOb->is_npc(), UNDEFINED),
    }) :));
}

void test_check_empty () {
    expect("check_empty removes empty coins", (: ({
        // check with currency
        testOb->add_currency("copper", 123),
        assert_equal(testOb->query_currencies(), ({ "copper", })),
        testOb->check_empty(),
        assert_regex(testOb, "/std/item/coins.coverage#[0-9]+"),
        testOb->add_currency("copper", -123),

        // check without currency
        assert_equal(testOb->query_currencies(), ({ })),
        testOb->check_empty(),
        assert_equal(testOb, UNDEFINED),
    }) :));
}

void test_coin_long () {
    expect("query_long_coin/query_long returns currency contents", (: ({
        assert_equal(testOb->query_currencies(), ({ })),
        assert_equal(testOb->query_long_coin(), "A pile of coins consisting of coins of no value."),
        assert_equal(testOb->query_long(), "A pile of coins consisting of coins of no value."),

        testOb->add_currency("copper", 123),
        assert_equal(testOb->query_currencies(), ({ "copper", })),
        assert_equal(testOb->query_long_coin(), "A pile of coins consisting of 123 copper."),
        assert_equal(testOb->query_long(), "A pile of coins consisting of 123 copper."),
    }) :));
}

void test_received () {
    expect("handle_receive gives coins to env and removes", (: ({
        // no currency on container
        assert_equal(this_object()->query_currencies(), ({ })),
        // currency on test ob
        testOb->add_currency("copper", 123),
        assert_equal(testOb->query_currencies(), ({ "copper" })),

        // container should receive coins
        assert_equal(testOb->handle_move(this_object()), 0), // coins remove before handle_move finishes
        assert_equal(this_object()->query_currencies(), ({ "copper" })),
        assert_equal(this_object()->query_currency("copper"), 123),
        // coins ob should be gone
        assert_equal(testOb, UNDEFINED),
    }) :));
}