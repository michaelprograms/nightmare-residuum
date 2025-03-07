inherit M_TEST;

/**
 * @var {"/std/ability/cost"} testOb
 */

#define CONFIG_MOCK "/std/ability/cost.c" & "/std/ability/config.mock.c"

void test_query_cost () {
    object mockConfig = new("/std/ability/config.mock.c");
    mockConfig->start_shadow(testOb);

    expect("handles cost", (: ({
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

    mockConfig->stop_shadow();
    if (mockConfig) destruct(mockConfig);
}