inherit M_TEST;

/**
 * @var {"/std/ability"} testOb
 */

nosave private int __MockLiving;
int is_living () { return __MockLiving; }

nosave private string __MockClass;
string query_class () { return __MockClass; }

// @TODO: clean this up with real test mock stats
int MockCharisma = 0, MockIntelligence = 0, MockPerception = 0;
int query_stat (string key) {
    if (key == "charisma") {
        return MockCharisma;
    } else if (key == "intelligence") {
        return MockIntelligence;
    } else if (key == "perception") {
        return MockPerception;
    } else {
        return 0;
    }
}
int MockLevel = 0;
int query_level () {
    return MockLevel;
}

void test_name () {
    expect("handles ability name", (: ({
        assert_equal(testOb->query_name(), "ability.coverage"),
    }) :));
}

int cValue1 = 0, cValue2 = 0;
void test_calculate () {
    MockIntelligence = 10;
    MockCharisma = 10;
    MockLevel = 10;

    expect("heals are calculated", (: ({
        // @TODO: remove these eventually
        testOb->set_powers(([ "theurgy": 1, "medicine": 1, ])),
        cValue1 = testOb->calculate_heal(this_object(), this_object(), 0),
        assert_equal(cValue1 > 0, 1),

        MockIntelligence = 100,
        MockCharisma = 100,
        // @TODO: remove these eventually
        testOb->set_powers(([ "theurgy": 123, "medicine": 123, ])),
        cValue2 = testOb->calculate_heal(this_object(), this_object(), 0),
        assert_equal(cValue2 >= cValue1, 1),
    }) :));

    expect("damage is calculated", (: ({
        testOb->set_powers(([ "psionic": 10, "ranged": 10, "brawl": 10, ])),
        cValue1 = testOb->calculate_damage(this_object(), this_object(), "head"),
        assert_equal(cValue1 > 0, 1),

        testOb->set_powers(([ "psionic": 123, "ranged": 123, "brawl": 123, ])),
        cValue2 = testOb->calculate_damage(this_object(), this_object(), "head"),
        assert_equal(cValue2 > cValue1, 1),

        __MockClass = "psionist",
        cValue1 = testOb->calculate_damage(this_object(), this_object(), "head"),
        assert_equal(cValue1 > 0, 1),

        __MockClass = "mystic",
        cValue1 = testOb->calculate_damage(this_object(), this_object(), "head"),
        assert_equal(cValue1 > 0, 1),
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

void test_ability_success () {
    expect("is_ability_successful behaves", (: ({
        // no type, fails
        assert_equal(testOb->is_ability_successful(this_object(), this_object()), 0),

        // attack type, should succeed
        testOb->set_type("attack"),
        testOb->set_powers(([ "psionic": 123, "ranged": 123, "brawl": 123 ])),
        assert_equal(testOb->is_ability_successful(this_object(), this_object()), 1),

        // attack type, should be 0-1
        MockIntelligence = 100,
        MockPerception = 10000,
        testOb->set_difficulty_factor(200),
        assert_equal(testOb->is_ability_successful(this_object(), this_object()) <= 1, 1),
        MockIntelligence = 0,
        MockPerception = 0,

        // utilities always pass
        testOb->set_type("utility"),
        assert_equal(testOb->is_ability_successful(this_object(), this_object()), 1),

        // utilities always pass
        testOb->set_type("heal"),
        assert_equal(testOb->is_ability_successful(this_object(), this_object()), 1),
    }) :));
}

void test_ability_use () {
    object char = new(STD_CHARACTER);
    object mockC1 = new("/std/npc.mock.c"); // TODO: this is weird, its not an NPC but this is the functionality we need

    char->set_name("test character");
    mockC1->start_shadow(char);

    expect("ability use ties it all together", (: ({
        testOb->set_type("attack"),

        // simple level requirement
        testOb->set_ability_requirements(([ "anyone": ([ "level": 10 ]) ])),

        // doesn't meet requirements
        $(char)->set_level(1),
        testOb->handle_ability_use($(char), 0),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "action", "You cannot do that." })),

        // meet requirements. but cooldown
        $(char)->set_level(10),
        $(char)->set_cooldown(testOb->query_name(), 1),
        testOb->handle_ability_use($(char), 0),
        assert_equal($(mockC1)->query_received_messages()[<1], ({ "action", "You are not yet ready to ability.coverage again." })),
    }) :));

    mockC1->stop_shadow();
    if (mockC1) destruct(mockC1);
    if (char) destruct(char);
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

        assert_catch((: testOb->set_difficulty_factor(UNDEFINED) :), "*Bad argument 1 to ability->set_difficulty_factor\n"),
    }) :));
}

void test_direct_verb_liv () {
    object ob;

    // setup test living object
    ob = new(STD_LIVING);
    ob->set_species("human");

    // setup test object
    __MockLiving = 1;

    expect("direct_verb_liv should handle applies", (: ({
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

void test_can_verb_rule () {
    // these functions call verb->can_verb_rule which is tested in verb.test.c
    expect("can_verb_rule should handle can_verb_* applies", (: ({
        assert_equal(testOb->can_verb_lvs("verb", "rule"), 1),
        assert_equal(testOb->can_verb_liv("verb", "rule"), 1),
        assert_equal(testOb->can_verb("verb", "rule"), 1),
    }) :));
}

void test_do_verb_applies () {
    expect("do_verb_liv is handled", (: ({
        // this error signifies that handle_ability_use was called
        assert_catch((: testOb->do_verb_liv(0, this_object()) :), "*Ability ability.coverage does not have an ability type set\n"),
    }) :));

    expect("do_verb_lvs is handled", (: ({
        // no args, no result
        assert_equal(testOb->do_verb_lvs(), UNDEFINED),

        testOb->set_targets(1),
        // this error signifies that handle_ability_use was called
        assert_catch((: testOb->do_verb_lvs(0, ({ this_object() })) :), "*Ability ability.coverage does not have an ability type set\n"),

        testOb->set_targets(3),
        // this error signifies that handle_ability_use was called
        assert_catch((: testOb->do_verb_lvs(0, ({ this_object(), this_object(), this_object() })) :), "*Ability ability.coverage does not have an ability type set\n"),
    }) :));

    expect("do_verb_rule is handled", (: ({
        // this error signifies that handle_ability_use was called
        assert_catch((: testOb->do_verb_rule() :), "*Ability ability.coverage does not have an ability type set\n"),
    }) :));
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