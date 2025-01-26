inherit M_TEST;

/**
 * @var {"/secure/sefun/override"} testOb
 */

void test_disabled_efuns () {
    expect("input_to throws error", (: ({
        assert_catch((: testOb->input_to() :), "*efun::input_to disabled\n"),
    }) :));
    expect("this_player throws error", (: ({
        assert_catch((: testOb->this_player() :), "*efun::this_player disabled\n"),
    }) :));
}

void test_efuns () {
    expect("users returns an array", (: ({
        assert_equal(arrayp(testOb->users()), 1),
    }) :));
}

void test_message () {
    object r1;
    object npc1, npc2;
    object mockNpc1, mockNpc2;

    r1 = new(STD_ROOM);
    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    mockNpc1 = new("/std/npc.mock.c");
    mockNpc2 = new("/std/npc.mock.c");
    npc1->handle_move(r1);
    npc2->handle_move(r1);

    expect("message sends to targets", (: ({
        assert_equal($(mockNpc1)->start_shadow($(npc1)), 1),
        assert_equal($(mockNpc2)->start_shadow($(npc2)), 1),

        // blank msg
        testOb->message("type", "", ({ $(mockNpc1), $(mockNpc1) })),
        assert_equal(/** @type {"/std/npc.mock"} */ ($(npc1))->query_received_messages(), ({ })),
        assert_equal(/** @type {"/std/npc.mock"} */ ($(npc2))->query_received_messages(), ({ })),

        // target msg
        testOb->message("type", "Message.", $(mockNpc1)),
        assert_equal(/** @type {"/std/npc.mock"} */ ($(npc1))->query_received_messages(), ({ ({ "type", "Message." }) })),
        assert_equal(/** @type {"/std/npc.mock"} */ ($(npc2))->query_received_messages(), ({ })),

        // room msg
        testOb->message("room", "Room message.", $(r1)),
        assert_equal(/** @type {"/std/npc.mock"} */ ($(npc1))->query_received_messages(), ({ ({ "type", "Message." }), ({ "room", "Room message." }) })),
        assert_equal(/** @type {"/std/npc.mock"} */ ($(npc2))->query_received_messages(), ({ ({ "room", "Room message." }) })),

        assert_equal($(mockNpc1)->stop_shadow(), 1),
        assert_equal($(mockNpc2)->stop_shadow(), 1),
    }) :));

    if (r1) destruct(r1);
    if (mockNpc1) destruct(mockNpc1);
    if (mockNpc2) destruct(mockNpc2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
}