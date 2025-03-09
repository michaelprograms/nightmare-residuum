inherit M_TEST;
inherit M_LEVELABLE;
inherit "/std/living/stats";
inherit "/std/living/vitals";

/**
 * @var {"/std/ability/cost"} testOb
 */

#define CONFIG_MOCK "/std/ability/cost.c" & "/std/ability/config.mock.c"
#define BODY_MOCK "/std/ability/cost.c" & "/std/living/body.mock.c"

void test_cost () {
    mapping cost = ([ "sp": 100, "mp": 100 ]);
    object mockBody = new("/std/living/body.mock.c");
    object mockConfig = new("/std/ability/config.mock.c");
    mockConfig->start_shadow(testOb);
    mockBody->start_shadow(testOb);

    expect("handles query_cost", (: ({
        assert_equal(testOb->query_cost(), ([ "sp": 0, "mp": 0 ])),

        /** @type {CONFIG_MOCK} */ (testOb)->set_powers(([ "ranged": 10 ])),
        assert_equal(testOb->query_cost(), ([ "sp": 10, "mp": 0 ])),

        /** @type {CONFIG_MOCK} */ (testOb)->set_powers(([ "brawl": 20 ])),
        assert_equal(testOb->query_cost(), ([ "sp": 20, "mp": 0 ])),

        /** @type {CONFIG_MOCK} */ (testOb)->set_powers(([ "psionic": 123 ])),
        assert_equal(testOb->query_cost(), ([ "sp": 0, "mp": 123 ])),

        /** @type {CONFIG_MOCK} */ (testOb)->set_powers(([ "brawl": 31, "psionic": 32 ])),
        assert_equal(testOb->query_cost(), ([ "sp": 31, "mp": 32 ])),
    }) :));

    expect("handles verify_cost", (: ({
        // setup this_object as too small
        set_level(1),
        set_stat("agility", 1),
        set_stat("intelligence", 1),
        update_vitals(1),
        assert_equal(this_object()->query_sp(), 15),
        assert_equal(this_object()->query_mp(), 15),
        // verify this_object is too small (not enough sp first)
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->verify_cost(this_object(), $(cost)), 0),
        set_stat("agility", 100),
        update_vitals(1),
        assert_equal(this_object()->query_sp(), 510),
        // verify this_object is too small (not enough mp second)
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->verify_cost(this_object(), $(cost)), 0),

        set_stat("intelligence", 100),
        update_vitals(1),
        // verify this_object is big enough
        assert_equal(this_object()->query_sp(), 510),
        assert_equal(this_object()->query_mp(), 510),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->verify_cost(this_object(), $(cost)), 1),
    }) :));

    expect("handles apply_cost", (: ({
        /** @type {BODY_MOCK} */ (testOb)->apply_cost(this_object(), $(cost)),
        assert_equal(this_object()->query_sp() < 510, 1),
        assert_equal(this_object()->query_mp() < 510, 1),
    }) :));

    mockBody->stop_shadow();
    if (mockBody) destruct(mockBody);
    mockConfig->stop_shadow();
    if (mockConfig) destruct(mockConfig);
}