inherit M_TEST;

/**
 * @var {"/std/ability/config"} testOb
 */

void test_type () {
    expect("handles setting and querying type", (: ({
        assert_equal(testOb->query_type(), UNDEFINED),

        testOb->set_type("attack"),
        assert_equal(testOb->query_type(), "attack"),

        testOb->set_type("heal"),
        assert_equal(testOb->query_type(), "heal"),

        testOb->set_type("utility"),
        assert_equal(testOb->query_type(), "utility"),

        assert_catch((: testOb->set_type("invalid") :), "*Bad argument 1 to ability/config->set_type\n"),
    }) :));
}

void test_powers () {
    expect("handles setting and querying skill powers", (: ({
        assert_equal(testOb->query_powers(), ([ ])),
        assert_equal(testOb->query_total_skill_power(), 0),

        testOb->set_powers(([ "brawl": 5, ])),
        assert_equal(testOb->query_powers(), ([ "brawl": 5, ])),
        assert_equal(testOb->query_total_skill_power(), 5),

        testOb->set_powers(([ "ranged": 5, "psionic": 5, ])),
        assert_equal(testOb->query_powers(), ([ "ranged": 5, "psionic": 5, ])),
        assert_equal(testOb->query_total_skill_power(), 10),

        assert_catch((: testOb->set_powers(UNDEFINED) :), "*Bad argument 1 to ability/config->set_powers\n"),
    }) :));
}