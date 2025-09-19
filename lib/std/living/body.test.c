inherit M_TEST;

/**
 * @var {"/std/living/body"} testOb
 */

#define BODY_MOCK "/std/living/body.c" & "/std/living/body.mock.c"

void test_gender () {
    expect("gender settable and queryable", (: ({
        assert_equal(testOb->query_gender(), "neither"),

        testOb->set_gender("male"),
        assert_equal(testOb->query_gender(), "male"),

        testOb->set_gender("female"),
        assert_equal(testOb->query_gender(), "female"),

        testOb->set_gender("neither"),
        assert_equal(testOb->query_gender(), "neither"),

        testOb->set_gender(0),
        assert_equal(testOb->query_gender(), "none"),

        testOb->set_gender("invalid"),
        assert_equal(testOb->query_gender(), "none"),
    }) :));
}

void test_species () {
    expect("species settable and queryable", (: ({
        assert_equal(testOb->query_species(), "unknown"),

        testOb->set_species("human"),
        assert_equal(testOb->query_species(), "human"),

        testOb->set_species("custodian"),
        assert_equal(testOb->query_species(), "custodian"),

        assert_catch((: testOb->set_species(UNDEFINED) :), "*Bad argument 1 to body->set_species\n"),
    }) :));
}

void test_worn_and_wielded () {
    expect("null worn is initialized", (: ({
        assert_equal(testOb->query_all_armor(), ({ })),
        store_variable("__Worn", UNDEFINED, testOb),
        assert_equal(testOb->query_all_armor(), ({ })),
    }) :));
    expect("null weapons are initialized", (: ({
        assert_equal(testOb->query_wielded_weapons(), ({ })),
        store_variable("__Wielded", UNDEFINED, testOb),
        assert_equal(testOb->query_wielded_weapons(), ({ })),
    }) :));
}

void test_limbs () {
    expect("species limbs are queryable", (: ({
        assert_equal(testOb->query_limbs(), ({ })),
        testOb->set_species("human"),

        assert_equal(sort_array(testOb->query_limbs(), 1), ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })),

        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 1, "pct": 100, "status": 0, "type": "FATAL", ])),
    }) :));
    expect("species limbs have wieldable", (: ({
        assert_equal(testOb->query_wieldable_limbs(), ({ "right hand", "left hand" })),
    }) :));
}

void test_random_limbs () {
    expect("species random limb behaves", (: ({
        assert_equal(testOb->query_random_limb(), 0),
        testOb->set_species("human"),

        // check random limb is valid
        assert_equal(member_array(testOb->query_random_limb(), ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })) > -1, 1),
        // run a second check
        assert_equal(member_array(testOb->query_random_limb(), ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })) > -1, 1),
    }) :));
}

void test_sever_and_restore () {
    object weapon1 = new(STD_WEAPON);
    object armor1 = new(STD_ARMOR);

    weapon1->set_type("blade");
    weapon1->set_hands(2);
    armor1->set_type("gloves");
    armor1->set_limbs(({ "left hand", "right hand" }));

    expect("limbs are severable", (: ({
        assert_equal(testOb->query_limbs(), ({ })),

        testOb->set_species("human"),
        assert_equal(sort_array(testOb->query_limbs(), 1), ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })),
        assert_equal(testOb->query_severed_limbs(), ({ })),

        // sever non-fatal, and verify weapon unwielded and armour unworn
        assert_equal(testOb->query_limb("left hand"), ([ "damage": 0, "maxdamage": 1, "pct": 25, "status": 0, "type": "WIELD" ])),
        testOb->handle_wield($(weapon1)),
        assert_equal($(weapon1)->query_wielded(), testOb),
        assert_equal(testOb->query_wielded_weapons(), ({ $(weapon1) })),
        testOb->handle_wear($(armor1)),
        assert_equal($(armor1)->query_worn(), testOb),
        assert_equal(testOb->query_all_armor(), ({ $(armor1) })),
        testOb->handle_limb_sever("left hand"),
        assert_equal(testOb->query_limb("left hand"), ([ "damage": -1, "maxdamage": 1, "pct": 25, "status": "severed", "type": "WIELD" ])),
        assert_equal($(weapon1)->query_wielded(), 0),
        assert_equal(testOb->query_wielded_weapons(), ({ })),

        // already severed non-fatal
        testOb->handle_limb_sever("left hand"),
        assert_equal(testOb->query_limb("left hand"), ([ "damage": -1, "maxdamage": 1, "pct": 25, "status": "severed", "type": "WIELD" ])),

        // sever fatal
        testOb->handle_limb_sever("torso"),
        assert_equal(testOb->query_limb("torso"), ([ "damage": -1, "maxdamage": 1, "pct": 100, "status": "severed", "type": "FATAL" ])),

        // verify hand and arm are attached
        assert_equal(testOb->query_limb("right hand"), ([ "damage": 0, "maxdamage": 1, "pct": 25, "status": 0, "type": "WIELD" ])),
        assert_equal(testOb->query_limb("right arm"), ([ "attach": "right hand", "damage": 0, "maxdamage": 1, "pct": 30, "status": 0, ])),
        // sever attached arm and hand
        testOb->handle_limb_sever("right arm"),
        assert_equal(testOb->query_limb("right hand"), ([ "damage": -1, "maxdamage": 1, "pct": 25, "status": "severed", "type": "WIELD" ])),
        assert_equal(testOb->query_limb("right arm"), ([ "attach": "right hand", "damage": -1, "maxdamage": 1, "pct": 30, "status": "severed", ])),
    }) :));
    expect("limbs are restorable", (: ({
        // hand and arm are severed
        assert_equal(testOb->query_limb("right hand"), ([ "damage": -1, "maxdamage": 1, "pct": 25, "status": "severed", "type": "WIELD" ])),
        assert_equal(testOb->query_limb("right arm"), ([ "attach": "right hand", "damage": -1, "maxdamage": 1, "pct": 30, "status": "severed", ])),

        // can't restore hand without arm
        assert_equal(testOb->handle_limb_restore("right hand"), -1),
        // still severed
        assert_equal(testOb->query_limb("right hand"), ([ "damage": -1, "maxdamage": 1, "pct": 25, "status": "severed", "type": "WIELD" ])),

        // restore arm
        assert_equal(testOb->handle_limb_restore("right arm"), 1),
        assert_equal(testOb->query_limb("right arm"), ([ "attach": "right hand", "damage": 0, "maxdamage": 1, "pct": 30, "status": 0, ])),
        // restore hand
        assert_equal(testOb->handle_limb_restore("right hand"), 1),
        assert_equal(testOb->query_limb("right hand"), ([ "damage": 0, "maxdamage": 1, "pct": 25, "status": 0, "type": "WIELD" ])),
        // already restored
        assert_equal(testOb->handle_limb_restore("right arm"), 0),
        assert_equal(testOb->handle_limb_restore("right hand"), 0),
    }) :));

    if (weapon1) destruct(weapon1);
    if (armor1) destruct(armor1);
}

void test_limbs_and_level () {
    object mockBody = new("/std/living/body.mock.c");

    mockBody->start_shadow(testOb);
    expect("setting species first has maxdamage", (: ({
        /** @type {BODY_MOCK} */ (testOb)->set_stat("endurance", 20),
        testOb->set_species("human"),
        /** @type {BODY_MOCK} */ (testOb)->set_level(10),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 342, "pct": 100, "status": 0, "type": "FATAL" ])),
        assert_equal(testOb->query_limb(UNDEFINED), 0),
    }) :));

    expect("setting level first has maxdamage", (: ({
        /** @type {BODY_MOCK} */ (testOb)->set_stat("endurance", 20),
        /** @type {BODY_MOCK} */ (testOb)->set_level(10),
        testOb->set_species("human"),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 342, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));

    expect("maxdamage to increase with endurance", (: ({
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 342, "pct": 100, "status": 0, "type": "FATAL" ])),

        /** @type {BODY_MOCK} */ (testOb)->set_stat("endurance", 40),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 562, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));

    mockBody->stop_shadow();
    if (mockBody) destruct(mockBody);
}

void test_injections () {
    object mockBody = new("/std/living/body.mock.c");

    expect("injections are addable and queryable", (: ({
        assert_equal(testOb->query_injections(), ([ ])),

        // adding
        testOb->add_injection("healing nanites", 5),
        assert_equal(testOb->query_injection("healing nanites"), 5),
        assert_equal(testOb->query_injections(), ([ "healing nanites": 5, ])),

        testOb->add_injection("healing nanites", 5),
        assert_equal(testOb->query_injections(), ([ "healing nanites": 10, ])),
        assert_equal(testOb->query_injection("healing nanites"), 10),

        testOb->add_injection("damaging nanites", 5),
        assert_equal(testOb->query_injections(), ([ "healing nanites": 10, "damaging nanites": 5, ])),
        assert_equal(testOb->query_injection("damaging nanites"), 5),

        testOb->add_injection("damaging nanites", 5),
        assert_equal(testOb->query_injections(), ([ "healing nanites": 10, "damaging nanites": 10, ])),
        assert_equal(testOb->query_injection("damaging nanites"), 10),

        // removing
        testOb->add_injection("healing nanites", -10),
        assert_equal(testOb->query_injections(), ([ "damaging nanites": 10, ])),
        assert_equal(testOb->query_injection("healing nanites"), 0),

        testOb->add_injection("damaging nanites", -10),
        assert_equal(testOb->query_injections(), ([ ])),
        assert_equal(testOb->query_injection("damaging nanites"), 0),
    }) :));

    expect("injections are handled by heart_beat", (: ({
        assert_equal($(mockBody)->start_shadow(testOb), 1),

        /** @type {BODY_MOCK} */ (testOb)->set_species("human"),
        /** @type {BODY_MOCK} */ (testOb)->set_level(1),
        testOb->add_injection("healing nanites", 10),
        testOb->add_injection("damaging nanites", 10),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites
        assert_equal(testOb->query_injections(), ([ "healing nanites": 10, "damaging nanites": 8, ])),
        assert_equal(testOb->query_injection("healing nanites"), 10),
        assert_equal(testOb->query_injection("damaging nanites"), 8),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites again
        assert_equal(testOb->query_injections(), ([ "healing nanites": 10, "damaging nanites": 6, ])),
        assert_equal(testOb->query_injection("healing nanites"), 10),
        assert_equal(testOb->query_injection("damaging nanites"), 6),

        // remove damaging nanites for healing nanites test
        testOb->add_injection("damaging nanites", -6),
        assert_equal(testOb->query_injection("damaging nanites"), 0),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites again
        assert_equal(testOb->query_injections(), ([ "healing nanites": 8, ])),
        assert_equal(testOb->query_injection("healing nanites"), 8),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites again
        assert_equal(testOb->query_injections(), ([ "healing nanites": 6, ])),
        assert_equal(testOb->query_injection("healing nanites"), 6),

        assert_equal($(mockBody)->stop_shadow(), 1),
    }) :));

    if (mockBody) destruct(mockBody);
}

void test_heal () {
    object mockBody = new("/std/living/body.mock.c");

    expect("heal restores hp/sp/mp and limb damage", (: ({
        assert_equal($(mockBody)->start_shadow(testOb), 1),

        testOb->set_species("human"),
        // verify initial state
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_hp(), 22),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_sp(), 12),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_mp(), 12),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // reduce vitals/limbs
        /** @type {BODY_MOCK} */ (testOb)->set_hp(1),
        /** @type {BODY_MOCK} */ (testOb)->set_sp(1),
        /** @type {BODY_MOCK} */ (testOb)->set_mp(1),
        testOb->handle_limb_heal("torso", -20),
        // verify reduced vitals/limbs
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_hp(), 1),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_sp(), 1),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_mp(), 1),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 20, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // negative heal does nothing
        testOb->heal(-10),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_hp(), 1),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_sp(), 1),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_mp(), 1),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 20, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // test heal vitals/limbs
        testOb->heal(10),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_hp(), 11),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_sp(), 11),
        assert_equal(/** @type {BODY_MOCK} */ (testOb)->query_mp(), 11),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 15, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // can't heal below zero
        testOb->handle_limb_heal("torso", 50),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // can't heal severed limbs
        testOb->handle_limb_sever("left hand"),
        testOb->handle_limb_heal("left hand", 50),
        assert_equal(testOb->query_limb("left hand"), ([ "damage": -1, "maxdamage": 6, "pct": 25, "status": "severed", "type": "WIELD" ])),

        assert_equal($(mockBody)->stop_shadow(), 1),
    }) :));

    if (mockBody) destruct(mockBody);
}

void test_damage () {
    object mockBody = new("/std/living/body.mock.c");
    object mockNpc;

    mockBody->set_level(5);
    mockNpc = new("/std/npc.mock.c");

    expect("heal restores hp/sp/mp and limb damage", (: ({
        assert_equal($(mockBody)->start_shadow(testOb), 1),

        testOb->set_species("human"),
        assert_equal(testOb->handle_damage(50, "left hand"), 50), // >= 100
        assert_equal(testOb->handle_damage(30, "right hand"), 30), // >= 75
        assert_equal(testOb->handle_damage(20, "right arm"), 20), // >= 50
        assert_equal($(mockBody)->stop_shadow(), 1),
    }) :));

    if (mockBody) destruct(mockBody);
    if (mockNpc) destruct(mockNpc);
}

void test_armor () {
    object armor1 = new(STD_ARMOR);
    object armor2 = new(STD_ARMOR);
    object weapon1 = new(STD_WEAPON);

    testOb->set_species("human");

    expect("querying armor behaves", (: ({
        assert_equal(testOb->query_all_armor(), ({ })),
        assert_equal(testOb->query_limb_armor("torso"), 0),

        $(armor1)->set_type("shirt"),
        $(armor1)->set_limbs(({ "torso" })),
        $(armor1)->set_ac(2),
        assert_equal(testOb->handle_wear($(armor1)), 1),
        assert_equal(testOb->query_all_armor(), ({ $(armor1) })),
        assert_equal(testOb->query_limb_armor("torso"), 2),
        assert_equal(testOb->handle_unwear($(armor1)), 1),

        $(armor1)->set_type("shield"),
        $(armor1)->set_limbs(({ "left arm", "left hand" })),
        assert_equal(testOb->handle_wear($(armor1)), 1),
        assert_equal(testOb->query_all_armor(), ({ $(armor1) })),
        assert_equal(testOb->query_limb_armor("left arm"), 2),
        assert_equal(testOb->query_limb_armor("left hand"), 2),
        assert_equal(testOb->query_worn_shield(), $(armor1)),
        // trying to wear another shield fails
        $(armor2)->set_type("shield"),
        $(armor2)->set_limbs(({ "left arm", "left hand" })),
        assert_equal(testOb->handle_wear($(armor2)), "You are already wearing a shield."),
        assert_equal(testOb->handle_unwear($(armor1)), 1),

        $(weapon1)->set_type("sword"),
        $(weapon1)->set_hands(2),
        assert_equal(testOb->handle_wield($(weapon1)), 1),
        assert_equal(testOb->handle_wear($(armor2)), "You cannot wear a shield while wielding a weapon in your left hand."),
    }) :));

    if (armor1) destruct(armor1);
    if (armor2) destruct(armor2);
    if (weapon1) destruct(weapon1);
}