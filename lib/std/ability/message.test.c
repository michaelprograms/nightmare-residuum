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

    expect("say responses should be handled", (: ({
        assert_equal($(mockConfig)->start_shadow(testOb), 1),
        assert_equal($(mockNPC1)->start_shadow($(npc1)), 1),
        assert_equal($(mockNPC2)->start_shadow($(npc2)), 1),
        assert_equal($(mockNPC3)->start_shadow($(npc3)), 1),

        // no messages yet
        assert_equal($(mockNPC1)->query_received_messages(), ({ })),
        assert_equal($(mockNPC2)->query_received_messages(), ({ })),
        assert_equal($(mockNPC3)->query_received_messages(), ({ })),

        // attack type
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("attack"),
        testOb->ability_message_attempt($(npc1), ({ $(npc2 )})),
        assert_equal($(mockNPC1)->query_received_messages(), ({ ({ "action", "You attempt to 0 Npc2!" }) })),
        assert_equal($(mockNPC2)->query_received_messages(), ({ ({ "action", "Npc1 attempts to 0 you!" }) })),
        assert_equal($(mockNPC3)->query_received_messages(), ({ ({ "action", "Npc1 attempts to 0 Npc2!" }) })),

        $(mockNPC1)->clear_received_messages(),
        $(mockNPC2)->clear_received_messages(),
        $(mockNPC3)->clear_received_messages(),

        // heal type
        /** @type {CONFIG_MOCK} */ (testOb)->set_type("heal"),
        testOb->ability_message_attempt($(npc1), ({ $(npc1), $(npc2 )})),
        assert_equal($(mockNPC1)->query_received_messages(), ({ ({ "action", "You attempt to 0 towards Npc2 and yourself." }) })),
        assert_equal($(mockNPC2)->query_received_messages(), ({ ({ "action", "Npc1 attempts to 0 towards you." }) })),
        assert_equal($(mockNPC3)->query_received_messages(), ({ ({ "action", "Npc1 attempts to 0 towards Npc2 and themself." }) })),

        assert_equal($(mockConfig)->stop_shadow(), 1),
        assert_equal($(mockNPC1)->stop_shadow(), 1),
        assert_equal($(mockNPC2)->stop_shadow(), 1),
        assert_equal($(mockNPC3)->stop_shadow(), 1),
    }) :));

    if (mockConfig) destruct(mockConfig);
    if (mockNPC1) destruct(mockNPC1);
    if (mockNPC2) destruct(mockNPC2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
    if (npc3) destruct(npc3);
}