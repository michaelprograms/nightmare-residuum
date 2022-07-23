inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/item/corpse.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_expire () {
    expect_function("handle_expire", testOb);

    expect("expire to remove corpse", (: ({
        assert(undefinedp(testOb), "==", 0),
        testOb->handle_expire(),
        assert(undefinedp(testOb), "==", 1),
    }) :));
}

private string testObFile;
private mixed *calloutInfo;
void test_received () {
    function_exists("handle_received", testOb);

    expect("handle_received sets expire timer", (: ({
        assert(testObFile = file_name(testOb), "regex", "/std/item/corpse#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test corpse
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert(sizeof(calloutInfo), "==", 1),
        assert(sizeof(calloutInfo[0]), "==", 3),
        assert(calloutInfo[0][0], "==", testOb),
    }) :));
}

void test_corpse () {
    expect_function("is_corpse", testOb);

    expect("is_corpse behaves", (: ({
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_corpse(), "==", 1),
        assert(testOb->is_character(), "==", UNDEFINED),
        assert(testOb->is_monster(), "==", UNDEFINED),
        assert(testOb->is_npc(), "==", UNDEFINED),
    }) :));
}

void test_body () {
    object liv = new(STD_LIVING);
    expect_function("setup_body", testOb);

    liv->set_name("tester");
    liv->set_id(({ "tester" }));
    liv->set_short("a tester");

    expect("body is setup to match", (: ({
        testOb->setup_body($(liv)),
        // verify we set corpse information
        assert(testOb->query_name(), "==", "corpse of tester"),
        assert(testOb->query_id(), "==", ({ "corpse", "corpseoftester", "corpse of tester" })),
        assert(testOb->query_short(), "==", "the corpse of a tester"),
    }) :));

    liv->set_short("a %^BOLD%^tester%^DEFAULT%^");
    expect("body handles short with DEFAULT", (: ({
        testOb->setup_body($(liv)),
        // verify we keep %^DEFAULT%^ instead of replacing to %^RESET%^
        assert(testOb->query_short(), "==", "the corpse of a %^BOLD%^tester%^DEFAULT%^"),
    }) :));

    liv->add_currency("copper", 12345);
    expect("body transfers currency", (: ({
        assert(present("coins", testOb), "==", 0),
        assert($(liv)->query_currency("copper"), "==", 12345),
        testOb->setup_body($(liv)),
        // verify coins moved to corpse
        assert(present("coins", testOb)->query_currency("copper"), "==", 12345),
        assert($(liv)->query_currency("copper"), "==", 0),
    }) :));

    destruct(liv);
}