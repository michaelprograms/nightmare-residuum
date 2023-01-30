inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/ability.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_ignore () {
    return ({
        "can_verb", "can_verb_liv", "can_verb_lvs",
        "do_verb_lvs", "do_verb_liv", "do_verb_rule",
    });
}

nosave private int __MockLiving;
int is_living () { return __MockLiving; }

void test_name () {
    expect_function("query_name", testOb);

    expect("handles ability name", (: ({
        assert(testOb->query_name(), "==", "ability"),
    }) :));
}

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("handles setting and querying type", (: ({
        assert(testOb->query_type(), "==", UNDEFINED),

        testOb->set_type("attack"),
        assert(testOb->query_type(), "==", "attack"),

        testOb->set_type("heal"),
        assert(testOb->query_type(), "==", "heal"),

        testOb->set_type("utility"),
        assert(testOb->query_type(), "==", "utility"),
    }) :));
}

void test_skill_powers () {
    expect_function("query_skill_powers", testOb);
    expect_function("set_skill_powers", testOb);

    expect("handles setting and querying skill powers", (: ({
        assert(testOb->query_skill_powers(), "==", ([ ])),

        testOb->set_skill_powers(([ "brawl": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "brawl": 5, ])),

        testOb->set_skill_powers(([ "ranged": 5, "psionic": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "ranged": 5, "psionic": 5, ])),
    }) :));
}

void test_weapons () {
    object liv, weaponBlade, weaponBlunt;

    expect_function("query_weapons", testOb);
    expect_function("set_weapons", testOb);
    expect_function("query_best_weapon", testOb);

    expect("handles setting and querying weapons", (: ({
        assert(testOb->query_weapons(), "==", ([ ])),

        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert(testOb->query_weapons(), "==", ([ "brawl": ({ 1 }), ])),

        testOb->set_weapons(([ "brawl": ({ 1 }), "blade": ({ 1, 2 }), "blunt": ({ 1, 2 }), ])),
        assert(testOb->query_weapons(), "==", ([ "brawl": ({ 1 }), "blade": ({ 1, 2 }), "blunt": ({ 1, 2 }), ])),
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
        assert($(liv)->query_species(), "==", "human"),
        assert($(liv)->query_wielded_weapons(), "==", ({ })),

        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert(testOb->query_best_weapon($(liv)), "==", UNDEFINED),

        // start with 1 handed
        testOb->set_weapons(([ "blunt": ({ 1 }), ])),
        // not wielded yet
        assert(testOb->query_best_weapon($(liv)), "==", UNDEFINED),
        // wield blade
        assert($(liv)->handle_wield($(weaponBlade)), "==", 1),
        assert(testOb->query_best_weapon($(liv)), "==", UNDEFINED),
        // wield blunt
        assert($(liv)->handle_wield($(weaponBlunt)), "==", 1),
        assert(testOb->query_best_weapon($(liv)), "==", $(weaponBlunt)),
        // change to 2 handed
        testOb->set_weapons(([ "blunt": ({ 2 }), ])),
        assert(testOb->query_best_weapon($(liv)), "==", UNDEFINED),
        assert($(liv)->handle_unwield($(weaponBlade)), "==", 1),
        assert($(liv)->handle_unwield($(weaponBlunt)), "==", 1),
        $(weaponBlunt)->set_hands(2),
        assert($(weaponBlunt)->query_hands(), "==", 2),
        assert($(liv)->handle_wield($(weaponBlunt)), "==", 1),
        assert(testOb->query_best_weapon($(liv)), "==", $(weaponBlunt)),
    }) :));

    if (liv) liv->handle_remove();
    if (weaponBlade) weaponBlade->handle_remove();
    if (weaponBlunt) weaponBlunt->handle_remove();
}

void test_targets () {
    expect_function("query_targets", testOb);
    expect_function("set_targets", testOb);

    expect("handles setting and querying targets", (: ({
        // default
        assert(testOb->query_targets(), "==", 1),

        testOb->set_targets(2),
        assert(testOb->query_targets(), "==", 2),

        testOb->set_targets(10),
        assert(testOb->query_targets(), "==", 10),
    }) :));
}

void test_difficulty_factor () {
    expect_function("query_difficulty_factor", testOb);
    expect_function("set_difficulty_factor", testOb);

    expect("handles difficulty factor", (: ({
        // defaults to 100
        assert(testOb->query_difficulty_factor(), "==", 100),

        // check lowering difficulty factor
        testOb->set_difficulty_factor(90),
        assert(testOb->query_difficulty_factor(), "==", 90),

        // check raising difficulty factor
        testOb->set_difficulty_factor(110),
        assert(testOb->query_difficulty_factor(), "==", 110),
    }) :));
}

void test_direct_verb_lib () {
    object ob;

    expect_function("direct_verb_liv", testOb);

    // setuip test living object
    ob = new(STD_LIVING);
    ob->set_species("human");

    // setup test object
    __MockLiving = 1;

    expect("handles ", (: ({
        // not enough arguments
        assert(testOb->direct_verb_liv(), "==", 0),
        assert(testOb->direct_verb_liv(1), "==", 0),

        // arguments but no type
        assert(testOb->direct_verb_liv(1, 1), "==", 0),
        // no type invalid against source
        assert(testOb->direct_verb_liv(1, this_object()), "==", 0),
        // no type invalid against target
        assert(testOb->direct_verb_liv(1, $(ob)), "==", 0),

        // set attack type
        testOb->set_type("attack"),
        assert(testOb->query_type(), "==", "attack"),
        // attack type but no living
        assert(testOb->direct_verb_liv(1, 1), "==", 0),
        // attack invalid against source
        assert(testOb->direct_verb_liv(1, this_object()), "==", 0),
        // attack target
        assert(testOb->direct_verb_liv(1, $(ob)), "==", 1),

        // set heal type
        testOb->set_type("heal"),
        assert(testOb->query_type(), "==", "heal"),
        // heal source
        assert(testOb->direct_verb_liv(1, this_object()), "==", 1),
        // heal target
        assert(testOb->direct_verb_liv(1, $(ob)), "==", 1),

        // set utility type
        testOb->set_type("utility"),
        assert(testOb->query_type(), "==", "utility"),
        // utility source
        assert(testOb->direct_verb_liv(1, this_object()), "==", 1),
        // utility target
        assert(testOb->direct_verb_liv(1, $(ob)), "==", 1),
    }) :));

    destruct(ob);
}

void test_ability_requirements () {
    expect_function("query_ability_requirements", testOb);
    expect_function("set_ability_requirements", testOb);

    expect("handles setting and querying requirements", (: ({
        // default value
        assert(testOb->query_ability_type(), "==", UNDEFINED),

        // check updated
        testOb->set_ability_requirements(([ "anyone": ([ "level": 1, ]), ])),
        assert(testOb->query_ability_requirements(), "==", ([ "anyone": ([ "level": 1, ]), ])),

        // check updated
        testOb->set_ability_requirements(([ "scoundrel": ([ "level": 5, ]), ])),
        assert(testOb->query_ability_requirements(), "==", ([ "scoundrel": ([ "level": 5, ]), ])),
    }) :));
}

void test_handle_help () {
    expect_function("handle_help", testOb);

    expect("handles additional formatting help file", (: ({
        // command.test.c tests basic formatting

        // should contain Type section always
        testOb->set_type("utility"),
        assert(testOb->query_type(), "==", "utility"),
        assert(regexp(testOb->handle_help(this_object()), "Type"), "==", 1),
        assert(regexp(testOb->handle_help(this_object()), "Utility"), "==", 1),

        // should contain ability requirements
        assert(regexp(testOb->handle_help(this_object()), "Anyone Requirements"), "==", 0),
        testOb->set_ability_requirements(([ "anyone": ([ "level": 1, "stats": ([ "strength": 1, ]), "skills": ([ "brawl attack": 1, ]) ]), ])),
        assert(regexp(testOb->handle_help(this_object()), "Anyone Requirements"), "==", 1),
        assert(regexp(testOb->handle_help(this_object()), "Level 1, 1 strength, 1 brawl attack"), "==", 1),

        // should contain weapon requirements
        assert(regexp(testOb->handle_help(this_object()), "Weapons"), "==", 0),
        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert(regexp(testOb->handle_help(this_object()), "Weapons"), "==", 1),
        assert(regexp(testOb->handle_help(this_object()), "one handed brawl"), "==", 1),
        // updated weapon requirements
        testOb->set_weapons(([ "blunt": ({ 1, 2 }), ])),
        assert(regexp(testOb->handle_help(this_object()), "one handed blunt, two handed blunt"), "==", 1),
    }) :));
}