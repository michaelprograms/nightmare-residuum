inherit M_TEST;

/**
 * @var {"/std/ability/target"} testOb
 */

#define CONFIG_MOCK "/std/ability/target.c" & "/std/ability/config.mock.c"

void test_targets () {
    expect("handles setting and querying targets", (: ({
        // default
        assert_equal(testOb->query_targets(), 1),

        testOb->set_targets(2),
        assert_equal(testOb->query_targets(), 2),

        testOb->set_targets(10),
        assert_equal(testOb->query_targets(), 10),

        assert_catch((: testOb->set_targets(0) :), "*Bad argument 1 to ability->set_targets\n"),
    }) :));
}

void test_verify_targets () {
    object npc1, npc2, room;
    object mockConfig = new("/std/ability/config.mock.c");

    room = new(STD_ROOM);
    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    mockConfig->start_shadow(testOb);

    expect("handles verifying targets", (: ({
        // no type set, no targets returned
        assert_equal(testOb->verify_targets(this_object(), ({ $(npc1), $(npc2) })), 0),

        /** @type {CONFIG_MOCK} */ (testOb)->set_type("attack"),
        $(npc1)->handle_move($(room)),
        $(npc2)->handle_move($(room)),
        assert_equal(testOb->verify_targets(this_object(), ({ $(npc2) })), ({ $(npc2) })),
    }) :));

    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
    if (room) destruct(room);
    mockConfig->stop_shadow();
    if (mockConfig) destruct(mockConfig);
}