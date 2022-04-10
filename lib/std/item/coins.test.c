inherit M_TEST;
inherit M_CURRENCY;
inherit M_CONTAINER;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/item/coins.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_currency () {
    expect_function("is_currency", testOb);

    expect("is_currency behaves", (: ({
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_currency(), "==", 1),
        assert(testOb->is_character(), "==", UNDEFINED),
        assert(testOb->is_monster(), "==", UNDEFINED),
        assert(testOb->is_npc(), "==", UNDEFINED),
    }) :));
}

void test_check_empty () {
    expect_function("check_empty", testOb);

    expect("check_empty removes empty coins", (: ({
        // check with currency
        testOb->add_currency("copper", 123),
        assert(testOb->query_currencies(), "==", ({ "copper", })),
        testOb->check_empty(),
        assert(testOb, "regex", "/std/item/coins#[0-9]+"),
        testOb->add_currency("copper", -123),

        // check without currency
        assert(testOb->query_currencies(), "==", ({ })),
        testOb->check_empty(),
        assert(testOb, "==", UNDEFINED),
    }) :));
}

void test_coin_long () {
    expect_function("query_long_coin", testOb);

    expect("query_long_coin/query_long returns currency contents", (: ({
        assert(testOb->query_currencies(), "==", ({ })),
        assert(testOb->query_long_coin(), "==", "A pile of coins consisting of coins of no value."),
        assert(testOb->query_long(), "==", "A pile of coins consisting of coins of no value."),

        testOb->add_currency("copper", 123),
        assert(testOb->query_currencies(), "==", ({ "copper", })),
        assert(testOb->query_long_coin(), "==", "A pile of coins consisting of 123 copper."),
        assert(testOb->query_long(), "==", "A pile of coins consisting of 123 copper."),
    }) :));
}

void test_received () {
    expect_function("handle_received", testOb);

    expect("handle_receive gives coins to env and removes", (: ({
        // no currency on container
        assert(this_object()->query_currencies(), "==", ({ })),
        // currency on test ob
        testOb->add_currency("copper", 123),
        assert(testOb->query_currencies(), "==", ({ "copper" })),

        // container should receive coins
        assert(testOb->handle_move(this_object()), "==", 0), // coins remove before handle_move finishes
        assert(this_object()->query_currencies(), "==", ({ "copper" })),
        assert(this_object()->query_currency("copper"), "==", 123),
        // coins ob should be gone
        assert(testOb, "==", UNDEFINED),
    }) :));
}