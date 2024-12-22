inherit M_TEST;

/**
 * @var {"/std/ability/requirements"} testOb
 */

void test_ability_requirements () {
    object char = new(STD_CHARACTER);

    expect("requirements are queryable, settable, and verifiable", (: ({
        assert_equal(testOb->query_ability_requirements(), UNDEFINED),
        assert_equal(testOb->verify_ability_requirements(0), 0),

        // no requirements
        testOb->set_ability_requirements(([ ])),
        assert_equal(testOb->query_ability_requirements(), ([ ])),
        // meets requirements
        assert_equal(testOb->verify_ability_requirements($(char)), 1),

        // single requirement
        testOb->set_ability_requirements(([ "anyone": ([ "level": 123 ]) ])),
        assert_equal(testOb->query_ability_requirements(), ([ "anyone": ([ "level": 123 ]) ])),
        // doesn't meet requirements
        $(char)->set_level(1),
        assert_equal(testOb->verify_ability_requirements($(char)), 0),
        // meet requirements
        $(char)->set_level(123),
        assert_equal(testOb->verify_ability_requirements($(char)), 1),

        // multiple requirements
        testOb->set_ability_requirements(([ "scoundrel": ([ "level": 10 ]), "psionist": ([ "level": 5 ]) ])),
        assert_equal(testOb->query_ability_requirements(), ([ "scoundrel": ([ "level": 10 ]), "psionist": ([ "level": 5 ]) ])),
        // doesn't meet requirements
        $(char)->set_level(4),
        $(char)->set_class("mystic"),
        assert_equal(testOb->verify_ability_requirements($(char)), 0),
        // meet requirements
        $(char)->set_level(5),
        $(char)->set_class("psionist"),
        assert_equal(testOb->verify_ability_requirements($(char)), 1),

        // STD_CHARACTER is not an STD_NPC
        testOb->set_ability_requirements(([ "NPC": ([ "level": 1 ]) ])),
        assert_equal(testOb->query_ability_requirements(), ([ "NPC": ([ "level": 1 ]) ])),
        assert_equal(testOb->verify_ability_requirements($(char)), 0),
        // doesn't meet requirements
        testOb->set_ability_requirements(([ "anyone": ([ "stats": ([ "strength": 25 ]) ]) ])),
        $(char)->set_stat("strength", 20),
        assert_equal(testOb->verify_ability_requirements($(char)), 0),
        // meets requirements
        $(char)->set_stat("strength", 25),
        assert_equal(testOb->verify_ability_requirements($(char)), 1),

        // doesn't meet requirements
        testOb->set_ability_requirements(([ "NPC": ([ "level": 1 ]) ])),
        assert_equal(testOb->query_ability_requirements(), ([ "NPC": ([ "level": 1 ]) ])),
        assert_equal(testOb->verify_ability_requirements($(char)), 0),
        // meets requirements as immortal
        $(char)->set_immortal(1),
        assert_equal(testOb->verify_ability_requirements($(char)), 1),
    }) :));

    if (char) destruct(char);
}