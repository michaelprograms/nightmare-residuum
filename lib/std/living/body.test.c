inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/body.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_gender () {
    expect_function("set_gender", testOb);
    expect_function("query_gender", testOb);

    expect("gender settable and queryable", (: ({
        assert(testOb->query_gender(), "==", "neither"),

        testOb->set_gender("male"),
        assert(testOb->query_gender(), "==", "male"),

        testOb->set_gender("female"),
        assert(testOb->query_gender(), "==", "female"),

        testOb->set_gender("neither"),
        assert(testOb->query_gender(), "==", "neither"),

        testOb->set_gender(0),
        assert(testOb->query_gender(), "==", "none"),

        testOb->set_gender("invalid"),
        assert(testOb->query_gender(), "==", "none"),
    }) :));
}

void test_species () {
    expect_function("set_species", testOb);
    expect_function("query_species", testOb);

    expect("species settable and queryable", (: ({
        assert(testOb->query_species(), "==", "unknown"),

        testOb->set_species("human"),
        assert(testOb->query_species(), "==", "human"),

        testOb->set_species("custodian"),
        assert(testOb->query_species(), "==", "custodian"),
    }) :));
}

void test_limbs () {
    expect_function("query_limbs", testOb);
    expect_function("query_limb", testOb);
    expect_function("query_random_limb", testOb);

    expect("species limbs are queryable", (: ({
        assert(testOb->query_limbs(), "==", ({ })),

        testOb->set_species("human"),
        assert(sort_array(testOb->query_limbs(), 1), "==", ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })),

        assert(testOb->query_limb("torso"), "==", ([ "damage": 0, "maxdamage": 1, "pct": 100, "status": 0, "type": "FATAL", ])),
    }) :));

    expect("species random limb behaves", (: ({
        // check random limb is valid
        assert(member_array(testOb->query_random_limb(), ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })) > -1, "==", 1),
        // run a second check
        assert(member_array(testOb->query_random_limb(), ({ "head", "left arm", "left foot", "left hand", "left leg", "right arm", "right foot", "right hand", "right leg", "torso" })) > -1, "==", 1),
    }) :));
}

void test_limbs_and_level () {
    destruct(testOb);
    testOb = new(STD_LIVING);
    expect("setting species first has maxdamage", (: ({
        testOb->set_stat("endurance", 20),
        testOb->set_species("human"),
        testOb->set_level(10),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 0, "maxdamage": 342, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));

    destruct(testOb);
    testOb = new(STD_LIVING);
    expect("setting level first has maxdamage", (: ({
        testOb->set_stat("endurance", 20),
        testOb->set_level(10),
        testOb->set_species("human"),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 0, "maxdamage": 342, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));

    destruct(testOb);
    testOb = new(STD_LIVING);
    expect("maxdamage to increase with endurance", (: ({
        testOb->set_stat("endurance", 20),
        testOb->set_level(10),
        testOb->set_species("human"),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 0, "maxdamage": 342, "pct": 100, "status": 0, "type": "FATAL" ])),

        testOb->set_stat("endurance", 40),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 0, "maxdamage": 562, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));

    destruct(testOb);
}

void test_injections () {
    expect_function("query_injections", testOb);
    expect_function("query_injection", testOb);
    expect_function("add_injection", testOb);

    expect("injections are addable and queryable", (: ({
        assert(testOb->query_injections(), "==", ([ ])),

        testOb->add_injection("healing nanites", 5),
        assert(testOb->query_injection("healing nanites"), "==", 5),
        assert(testOb->query_injections(), "==", ([ "healing nanites": 5, ])),

        testOb->add_injection("healing nanites", 5),
        assert(testOb->query_injections(), "==", ([ "healing nanites": 10, ])),
        assert(testOb->query_injection("healing nanites"), "==", 10),

        testOb->add_injection("damaging nanites", 5),
        assert(testOb->query_injections(), "==", ([ "healing nanites": 10, "damaging nanites": 5, ])),
        assert(testOb->query_injection("damaging nanites"), "==", 5),

        testOb->add_injection("damaging nanites", 5),
        assert(testOb->query_injections(), "==", ([ "healing nanites": 10, "damaging nanites": 10, ])),
        assert(testOb->query_injection("damaging nanites"), "==", 10),
    }) :));

    destruct(testOb);
    testOb = new(STD_LIVING);
    expect("injections are handled by heart_beat", (: ({
        testOb->set_race("human"),
        testOb->set_level(1),
        testOb->add_injection("healing nanites", 10),
        testOb->add_injection("damaging nanites", 10),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites
        assert(testOb->query_injections(), "==", ([ "healing nanites": 10, "damaging nanites": 8, ])),
        assert(testOb->query_injection("healing nanites"), "==", 10),
        assert(testOb->query_injection("damaging nanites"), "==", 8),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites again
        assert(testOb->query_injections(), "==", ([ "healing nanites": 10, "damaging nanites": 6, ])),
        assert(testOb->query_injection("healing nanites"), "==", 10),
        assert(testOb->query_injection("damaging nanites"), "==", 6),

        // remove damaging nanites for healing nanites test
        testOb->add_injection("damaging nanites", -6),
        assert(testOb->query_injection("damaging nanites"), "==", 0),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites again
        assert(testOb->query_injections(), "==", ([ "healing nanites": 8, ])),
        assert(testOb->query_injection("healing nanites"), "==", 8),

        // heart_beat for handle_injections
        testOb->heart_beat(),

        // damaging nanites work before healing nanites again
        assert(testOb->query_injections(), "==", ([ "healing nanites": 6, ])),
        assert(testOb->query_injection("healing nanites"), "==", 6),
    }) :));
}

void test_heal () {
    expect_function("heal", testOb);

    destruct(testOb);
    testOb = new(STD_LIVING);

    expect("heal restores hp", (: ({
        testOb->set_species("human"),
        // verify initial state
        assert(testOb->query_hp(), "==", 22),
        assert(testOb->query_sp(), "==", 12),
        assert(testOb->query_mp(), "==", 12),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 0, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // reduce vitals/limbs
        testOb->set_hp(1),
        testOb->set_sp(1),
        testOb->set_mp(1),
        testOb->handle_limb_heal("torso", -20),
        // verify reduced vitals/limbs
        assert(testOb->query_hp(), "==", 1),
        assert(testOb->query_sp(), "==", 1),
        assert(testOb->query_mp(), "==", 1),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 20, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // test heal vitals/limbs
        testOb->heal(10),
        assert(testOb->query_hp(), "==", 11),
        assert(testOb->query_sp(), "==", 11),
        assert(testOb->query_mp(), "==", 11),
        assert(testOb->query_limb("torso"), "==", ([ "damage": 15, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));
}