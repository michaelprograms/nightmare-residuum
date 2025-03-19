inherit M_TEST;

/**
 * @var {"/std/ability"} testOb
 */

nosave private int __MockLiving;
int is_living () { return __MockLiving; }

// @TODO: clean this up with real test mock stats
int MockIntelligence = 0;
int query_stat (string key) {
    if (key == "intelligence") {
        return MockIntelligence;
    }
}

void test_name () {
    expect("handles ability name", (: ({
        assert_equal(testOb->query_name(), "ability.coverage"),
    }) :));
}

int cValue1 = 0, cValue2 = 0;
void test_calculate () {
    MockIntelligence = 10;

    expect("heals are calculated", (: ({
        cValue1 = testOb->calculate_heal(this_object(), this_object(), 0),
        assert_equal(cValue1 > 0, 1),

        // @TODO: remove these eventually
        testOb->set_powers(([ "theurgy": 1, "medicine": 1, ])),
        cValue2 = testOb->calculate_heal(this_object(), this_object(), 0),
        assert_equal(cValue2 > cValue1, 1),
    }) :));
}

void test_weapons () {
    object liv, weaponBlade, weaponBlunt;

    expect("handles setting and querying weapons", (: ({
        assert_equal(testOb->query_weapons(), ([ ])),

        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert_equal(testOb->query_weapons(), ([ "brawl": ({ 1 }), ])),

        testOb->set_weapons(([ "brawl": ({ 1 }), "blade": ({ 1, 2 }), "blunt": ({ 1, 2 }), ])),
        assert_equal(testOb->query_weapons(), ([ "brawl": ({ 1 }), "blade": ({ 1, 2 }), "blunt": ({ 1, 2 }), ])),
    }) :));

    liv = new(STD_LIVING);
    liv->set_species("human");
    weaponBlade = new(STD_WEAPON);
    weaponBlade->set_type("blade");
    weaponBlade->set_wc(5);
    weaponBlade->set_name("test blade");
    weaponBlunt = new(STD_WEAPON);
    weaponBlunt->set_type("blunt");
    weaponBlunt->set_wc(5);
    weaponBlunt->set_name("test blunt");

    expect("handles setting and querying weapons", (: ({
        assert_equal($(liv)->query_species(), "human"),
        assert_equal($(liv)->query_wielded_weapons(), ({ })),

        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert_equal(testOb->query_best_weapon($(liv)), UNDEFINED),

        // start with 1 handed
        testOb->set_weapons(([ "blunt": ({ 1 }), ])),
        // not wielded yet
        assert_equal(testOb->query_best_weapon($(liv)), UNDEFINED),
        // wield blade
        assert_equal($(liv)->handle_wield($(weaponBlade)), 1),
        assert_equal(testOb->query_best_weapon($(liv)), UNDEFINED),
        // wield blunt
        assert_equal($(liv)->handle_wield($(weaponBlunt)), 1),
        assert_equal(testOb->query_best_weapon($(liv)), $(weaponBlunt)),
        // change to 2 handed
        testOb->set_weapons(([ "blunt": ({ 2 }), ])),
        assert_equal(testOb->query_best_weapon($(liv)), UNDEFINED),
        assert_equal($(liv)->handle_unwield($(weaponBlade)), 1),
        assert_equal($(liv)->handle_unwield($(weaponBlunt)), 1),
        $(weaponBlunt)->set_hands(2),
        assert_equal($(weaponBlunt)->query_hands(), 2),
        assert_equal($(liv)->handle_wield($(weaponBlunt)), 1),
        assert_equal(testOb->query_best_weapon($(liv)), $(weaponBlunt)),
    }) :));

    if (liv) liv->handle_remove();
    if (weaponBlade) weaponBlade->handle_remove();
    if (weaponBlunt) weaponBlunt->handle_remove();
}

void test_cooldown () {
    expect("handles setting and querying cooldown", (: ({
        // default
        assert_equal(testOb->query_cooldown(), 1),

        testOb->set_cooldown(2),
        assert_equal(testOb->query_cooldown(), 2),

        testOb->set_cooldown(10),
        assert_equal(testOb->query_cooldown(), 10),

        testOb->set_cooldown(0),
        assert_equal(testOb->query_cooldown(), 0),

        assert_catch((: testOb->set_cooldown(-1) :), "*Bad argument 1 to ability->set_cooldown\n"),
    }) :));
}

void test_difficulty_factor () {
    expect("handles difficulty factor", (: ({
        // defaults to 100
        assert_equal(testOb->query_difficulty_factor(), 100),

        // check lowering difficulty factor
        testOb->set_difficulty_factor(90),
        assert_equal(testOb->query_difficulty_factor(), 90),

        // check raising difficulty factor
        testOb->set_difficulty_factor(110),
        assert_equal(testOb->query_difficulty_factor(), 110),
    }) :));
}

void test_can_verb_rule () {
    // these functions call verb->can_verb_rule which is tested in verb.test.c
    expect("can_verb_rule should handle can_verb_* applies", (: ({
        assert_equal(testOb->can_verb_lvs("verb", "rule"), 1),
        assert_equal(testOb->can_verb_liv("verb", "rule"), 1),
        assert_equal(testOb->can_verb("verb", "rule"), 1),
    }) :));
}

void test_direct_verb_lib () {
    object ob;

    // setuip test living object
    ob = new(STD_LIVING);
    ob->set_species("human");

    // setup test object
    __MockLiving = 1;

    expect("handles ", (: ({
        // not enough arguments
        assert_equal(testOb->direct_verb_liv(), 0),
        assert_equal(testOb->direct_verb_liv(1), 0),

        // arguments but no type
        assert_equal(testOb->direct_verb_liv(1, 1), 0),
        // no type invalid against source
        assert_equal(testOb->direct_verb_liv(1, this_object()), 0),
        // no type invalid against target
        assert_equal(testOb->direct_verb_liv(1, $(ob)), 0),

        // set attack type
        testOb->set_type("attack"),
        assert_equal(testOb->query_type(), "attack"),
        // attack type but no living
        assert_equal(testOb->direct_verb_liv(1, 1), 0),
        // attack invalid against source
        assert_equal(testOb->direct_verb_liv(1, this_object()), 0),
        // attack target
        assert_equal(testOb->direct_verb_liv(1, $(ob)), 1),

        // set heal type
        testOb->set_type("heal"),
        assert_equal(testOb->query_type(), "heal"),
        // heal source
        assert_equal(testOb->direct_verb_liv(1, this_object()), 1),
        // heal target
        assert_equal(testOb->direct_verb_liv(1, $(ob)), 1),

        // set utility type
        testOb->set_type("utility"),
        assert_equal(testOb->query_type(), "utility"),
        // utility source
        assert_equal(testOb->direct_verb_liv(1, this_object()), 1),
        // utility target
        assert_equal(testOb->direct_verb_liv(1, $(ob)), 1),
    }) :));

    destruct(ob);
}

void test_ability_requirements () {
    expect("handles setting and querying requirements", (: ({
        // default value
        assert_equal(testOb->query_ability_requirements(), ([ ])),

        // check updated
        testOb->set_ability_requirements(([ "anyone": ([ "level": 1, ]), ])),
        assert_equal(testOb->query_ability_requirements(), ([ "anyone": ([ "level": 1, ]), ])),

        // check updated
        testOb->set_ability_requirements(([ "scoundrel": ([ "level": 5, ]), ])),
        assert_equal(testOb->query_ability_requirements(), ([ "scoundrel": ([ "level": 5, ]), ])),
    }) :));
}

void test_handle_help () {
    expect("handles additional formatting help file", (: ({
        // command.test.c tests basic formatting

        // should contain Type section always
        testOb->set_type("utility"),
        assert_equal(testOb->query_type(), "utility"),
        assert_equal(regexp(testOb->handle_help(this_object()), "Type"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "Utility"), 1),

        // should contain ability requirements
        assert_equal(regexp(testOb->handle_help(this_object()), "Anyone Requirements"), 0),
        testOb->set_ability_requirements(([ "anyone": ([ "level": 1, "stats": ([ "strength": 1, ]), "skills": ([ "brawl attack": 1, ]) ]), ])),
        assert_equal(regexp(testOb->handle_help(this_object()), "Anyone Requirements"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "Level 1, 1 strength, 1 brawl attack"), 1),

        // should contain weapon requirements
        assert_equal(regexp(testOb->handle_help(this_object()), "Weapons"), 0),
        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert_equal(regexp(testOb->handle_help(this_object()), "Weapons"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "one handed brawl"), 1),
        // updated weapon requirements
        testOb->set_weapons(([ "blunt": ({ 1, 2 }), ])),
        assert_equal(regexp(testOb->handle_help(this_object()), "one handed blunt, two handed blunt"), 1),

        // cooldown
        testOb->set_cooldown(2),
        assert_equal(regexp(testOb->handle_help(this_object()), "2 rounds"), 1),
    }) :));
}