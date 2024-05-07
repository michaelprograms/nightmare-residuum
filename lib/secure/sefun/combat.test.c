inherit M_TEST;

void test_initiate_combat () {
    object npc1, npc2;
    object mockNpc1, mockNpc2;

    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    mockNpc1 = new("/std/mock/npc.c");
    mockNpc2 = new("/std/mock/npc.c");

    expect("hostilities should ensue", (: ({
        assert_equal($(mockNpc1)->start_shadow($(npc1)), 1),
        assert_equal($(mockNpc2)->start_shadow($(npc2)), 1),

        assert_equal($(npc1)->query_hostiles(), ({ })),
        assert_equal($(npc2)->query_hostiles(), ({ })),

        testOb->initiate_combat($(npc1), $(npc2)),
        assert_equal($(npc1)->query_hostiles(), ({ $(npc2) })),
        assert_equal($(npc2)->query_hostiles(), ({ $(npc1) })),

        assert_equal($(npc1)->query_received_messages(), ({ "attack", "You attack 0!" })),
        assert_equal($(npc2)->query_received_messages(), ({  "attack", "0 attacks you!" })),

        assert_equal($(mockNpc1)->stop_shadow(), 1),
        assert_equal($(mockNpc2)->stop_shadow(), 1),
    }) :));

    if (mockNpc1) destruct(mockNpc1);
    if (mockNpc2) destruct(mockNpc2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
}