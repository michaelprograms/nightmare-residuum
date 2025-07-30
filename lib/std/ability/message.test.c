inherit M_TEST;

/**
 * @var {"/std/ability/message"} testOb
 */

#define CONFIG_MOCK "/std/ability/message.c" & "/std/ability/config.mock.c"

void test_messages () {
    object mockConfig = new("/std/ability/config.mock.c");
    object mockNPC1 = new("/std/npc.mock.c");
    object mockNPC2 = new("/std/npc.mock.c");
    object mockNPC3 = new("/std/npc.mock.c");
    object npc1 = new(STD_NPC);
    object npc2 = new(STD_NPC);
    object npc3 = new(STD_NPC);
    object r = new(STD_ROOM);

    npc1->set_name("npc1");
    npc2->set_name("npc2");
    npc3->set_name("npc3");

    npc1->handle_move(r);
    npc2->handle_move(r);
    npc3->handle_move(r);

    mockConfig->start_shadow(testOb);
    mockNPC1->start_shadow(npc1);
    mockNPC2->start_shadow(npc2);
    mockNPC3->start_shadow(npc3);

    expect("attempt messages are handled", (: ({
        // attack type
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("attack"),
        testOb->ability_message_attempt($(npc1), ({ $(npc2 )})),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "action", "You attempt to 0 Npc2!" })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "action", "Npc1 attempts to 0 you!" })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "action", "Npc1 attempts to 0 Npc2!" })),

        // heal type
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("heal"),
        testOb->ability_message_attempt($(npc1), ({ $(npc1), $(npc2 )})),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "action", "You attempt to 0 towards Npc2 and yourself." })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "action", "Npc1 attempts to 0 towards you." })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "action", "Npc1 attempts to 0 towards Npc2 and themself." })),
    }) :));

    mockNPC1->clear_received_messages();
    mockNPC2->clear_received_messages();
    mockNPC3->clear_received_messages();

    expect("fail messages are handled", (: ({
        // attack type, targeting another
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("attack"),
        testOb->ability_message_fail($(npc1), $(npc2), 0),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "ability miss", "You miss your 0 attempt on Npc2!" })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "ability miss", "Npc1 misses their 0 attempt on you!" })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "ability miss", "Npc1 misses their 0 attempt on Npc2!" })),

        // heal type, targeting themself
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("heal"),
        testOb->ability_message_fail($(npc1), $(npc1), 0),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "ability miss", "Your 0 fails to affect yourself." })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "ability miss", "Npc1's 0 fails to affect themself." })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "ability miss", "Npc1's 0 fails to affect themself." })),

        // heal type, targeting another
        testOb->ability_message_fail($(npc1), $(npc2), 0),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "ability miss", "Your 0 fails to affect Npc2." })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "ability miss", "Npc1's 0 fails to affect you." })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "ability miss", "Npc1's 0 fails to affect Npc2." })),
    }) :));

    mockNPC1->clear_received_messages();
    mockNPC2->clear_received_messages();
    mockNPC3->clear_received_messages();

    expect("success messages are handled", (: ({
        // attack type, targeting another, no limb
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("attack"),
        testOb->ability_message_success($(npc1), $(npc2), 0),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "action", "You 0 Npc2!" })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "action", "Npc1 0s you!" })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "action", "Npc1 0s Npc2!" })),

        // attack type, targeting another's limb
        testOb->ability_message_success($(npc1), $(npc2), "torso"),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "action", "You 0 Npc2's torso!" })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "action", "Npc1 0s your torso!" })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "action", "Npc1 0s Npc2's torso!" })),

        // heal type, targeting themself
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("heal"),
        testOb->ability_message_success($(npc1), $(npc1), 0),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "action", "You 0 towards yourself effectively." })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "action", "Npc1 0s towards themself effectively." })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "action", "Npc1 0s towards themself effectively." })),

        // heal type, targeting another
        testOb->ability_message_success($(npc1), $(npc2), 0),
        assert_equal($(mockNPC1)->query_received_messages()[<1], ({ "action", "You 0 towards Npc2 effectively." })),
        assert_equal($(mockNPC2)->query_received_messages()[<1], ({ "action", "Npc1 0s towards you effectively." })),
        assert_equal($(mockNPC3)->query_received_messages()[<1], ({ "action", "Npc1 0s towards Npc2 effectively." })),
    }) :));

    mockConfig->stop_shadow();
    mockNPC1->stop_shadow();
    mockNPC2->stop_shadow();
    mockNPC3->stop_shadow();

    if (mockConfig) destruct(mockConfig);
    if (mockNPC1) destruct(mockNPC1);
    if (mockNPC2) destruct(mockNPC2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
    if (npc3) destruct(npc3);
}