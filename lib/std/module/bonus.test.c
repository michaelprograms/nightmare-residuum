inherit M_TEST;
inherit STD_LIVING;

/**
 * @var {"/std/module/bonus"} testOb
 */

void test_singular_bonus () {
    expect("null bonuses are initialized", (: ({
        assert_equal(testOb->query_bonuses(), ([ ])),
        store_variable("__Bonuses", UNDEFINED, testOb),
        assert_equal(testOb->query_bonuses(), ([ ])),
    }) :));

    expect("handles setting and querying bonus", (: ({
        assert_equal(testOb->query_bonus("strength"), 0),
        assert_equal(testOb->query_bonus("luck"), 0),

        assert_equal(testOb->set_bonus("strength", 1), 1),
        assert_equal(testOb->query_bonus("strength"), 1),

        assert_equal(testOb->set_bonus("strength", 2), 2),
        assert_equal(testOb->query_bonus("strength"), 2),

        assert_equal(testOb->set_bonus("luck", 3), 3),
        assert_equal(testOb->query_bonus("luck"), 3),
    }) :));
}

void test_multiple_bonuses () {
    expect("handles setting and querying bonuses", (: ({
        assert_equal(testOb->query_bonuses(), ([ ])),

        assert_equal(testOb->set_bonuses(([ "strength": 1 ])), ([ "strength": 1, ])),
        assert_equal(testOb->query_bonuses(), ([ "strength": 1, ])),

        assert_equal(testOb->set_bonuses(([ "strength": 2 ])), ([ "strength": 2, ])),
        assert_equal(testOb->query_bonuses(), ([ "strength": 2, ])),

        assert_equal(testOb->set_bonuses(([ "luck": 3 ])), ([ "strength": 2, "luck": 3, ])),
        assert_equal(testOb->query_bonuses(), ([ "strength": 2, "luck": 3, ])),
    }) :));
}

nosave private mapping __BonusesApplied = ([ ]);

// Listen for receive/release events from the room
void add_stat_bonus (string stat, int n) {
    __BonusesApplied[stat] = n;
}

void test_apply_and_remove_bonuses () {
    expect("bonuses are applied and removed ", (: ({
        // set bonuses
        assert_equal(testOb->set_bonuses(([ "strength": 123, "charisma": 123, ])), ([ "strength": 123, "charisma": 123, ])),
        assert_equal(testOb->query_bonuses(), ([ "strength": 123, "charisma": 123, ])),

        // apply bonuses
        testOb->apply_bonus(this_object()),
        // verify bonuses were applied
        assert_equal(__BonusesApplied, ([ "strength": 123, "charisma": 123, ])),

        // apply bonuses
        testOb->remove_bonus(this_object()),
        // verify bonuses were reversed
        assert_equal(__BonusesApplied, ([ "strength": -123, "charisma": -123, ])),
    }) :));
}