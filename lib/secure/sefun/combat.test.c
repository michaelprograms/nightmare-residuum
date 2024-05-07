inherit M_TEST;

void test_initiate_combat () {
    object npc1, npc2;

    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    expect("hostilities should ensue", (: ({
        assert_equal($(npc1)->query_hostiles(), ({ })),
        assert_equal($(npc2)->query_hostiles(), ({ })),

        testOb->initiate_combat($(npc1), $(npc2)),
        assert_equal($(npc1)->query_hostiles(), ({ $(npc2) })),
        assert_equal($(npc2)->query_hostiles(), ({ $(npc1) })),
    }) :));

    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
}