inherit M_TEST;

/**
 * @var {"/std/ability/target"} testOb
 */

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
    object npc1, npc2;
    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);

    expect("handles verifying targets", (: ({
        // no type set, no targets returned
        assert_equal(testOb->verify_targets(this_object(), ({ $(npc1), $(npc2) })), 0),
    }) :));

    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
}