inherit M_TEST;

/**
 * @var {"/secure/sefun/helper"} testOb
 */

void test_living_helpers () {
    object character = new(STD_CHARACTER);
    object npc = new(STD_NPC);

    expect("living helpers returns appropriately", (: ({
        assert_equal(testOb->livingp($(character)), 1),
        assert_equal(testOb->npcp($(character)), 0),
        assert_equal(testOb->characterp($(character)), 1),
        assert_equal(testOb->immortalp($(character)), 0),

        assert_equal(testOb->livingp($(npc)), 1),
        assert_equal(testOb->npcp($(npc)), 1),
        assert_equal(testOb->characterp($(npc)), 0),
        assert_equal(testOb->immortalp($(npc)), 0),
    }) :));

    if (character) destruct(character);
    if (npc) destruct(npc);
}