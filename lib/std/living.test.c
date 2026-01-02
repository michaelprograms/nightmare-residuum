inherit M_TEST;
inherit M_CONTAINER;

/**
 * @var {"/std/living"} testOb
 */

void test_living () {
    expect("is_living returns true", (: ({
        assert_equal(testOb->is_living(), 1),
        assert_equal(npcp(testOb), 0),
        assert_equal(characterp(testOb), 0),
    }) :));
}

void test_name () {
    expect("name updates long", (: ({
        assert_equal(testOb->query_long(), ""),

        testOb->set_name("test"),
        assert_equal(testOb->query_long(), "Test is an unknown."),

        testOb->set_gender("male"),
        assert_equal(testOb->query_long(), "Test is a male unknown."),

        testOb->set_attribute("build", "a"),
        testOb->set_attribute("complexion", "b"),
        testOb->set_attribute("eye", "c"),
        testOb->set_attribute("hair", "d"),
        testOb->set_attribute("height", "e"),
        assert_equal(testOb->query_long(), "Test is a male unknown with c eyes, e units tall, a b complexion, an a build, and d hair."),
    }) :));
}

void test_level () {
    expect("level updates vitals", (: ({
        testOb->set_species("human"),
        assert_equal(testOb->query_species(), "human"),
        assert_equal(testOb->query_level(), 1),
        assert_equal(testOb->query_hp(), 22),
        assert_equal(testOb->query_sp(), 12),
        assert_equal(testOb->query_mp(), 12),
        assert_equal(testOb->query_max_hp(), 22),
        assert_equal(testOb->query_max_sp(), 12),
        assert_equal(testOb->query_max_mp(), 12),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 0, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // adjust HP
        testOb->set_hp(1),
        testOb->set_sp(1),
        testOb->set_mp(1),
        assert_equal(testOb->query_hp(), 1),
        assert_equal(testOb->query_sp(), 1),
        assert_equal(testOb->query_mp(), 1),
        testOb->handle_limb_heal("torso", -10),
        assert_equal(testOb->query_limb("torso"), ([ "damage": 10, "maxdamage": 23, "pct": 100, "status": 0, "type": "FATAL" ])),

        // advance level
        testOb->set_level(2),
        // vitals are updated and healed
        assert_equal(testOb->query_hp(), 33),
        assert_equal(testOb->query_sp(), 18),
        assert_equal(testOb->query_mp(), 18),
        assert_equal(testOb->query_max_hp(), 33),
        assert_equal(testOb->query_max_sp(), 18),
        assert_equal(testOb->query_max_mp(), 18),
        // damage to limb remains, but maxdamage updated
        assert_equal(testOb->query_limb("torso"), ([ "damage": 10, "maxdamage": 34, "pct": 100, "status": 0, "type": "FATAL" ])),
    }) :));
}

void test_parser_applies () {
    object r = new(STD_ROOM);
    object ob = new(STD_OBJECT);

    expect("inject parser apply behaves", (: ({
        assert_equal(testOb->indirect_inject_obj_into_liv(), 1),
    }) :));

    expect("look at living apply behaves", (: ({
        // same environment (no environment)
        assert_equal(testOb->direct_look_at_liv(), 1),
        assert_equal(testOb->direct_look_liv(), 1),

        // different environments (one no env, one room)
        assert_equal(testOb->handle_move($(r)), 1),
        assert_equal(testOb->direct_look_at_liv(), 0),
        assert_equal(testOb->direct_look_liv(), 0),
    }) :));

    expect("give obj to living behaves", (: ({
        assert_equal(testOb->indirect_give_obj_to_liv($(ob), this_object(), 0, 0), 1),

        assert_equal($(ob)->handle_move(this_object()), 1),
        assert_equal(testOb->indirect_give_obj_to_liv($(ob), this_object(), 0, 0), "You already have that."),
    }) :));

    if (r) destruct(r);
}

void test_heart_beat () {
    object r = new(STD_ROOM);

    testOb->set_species("human");
    testOb->set_level(10);

    expect("living heart beat behaves", (: ({
        assert_equal(testOb->handle_move($(r)), 1),

        assert_equal(testOb->query_hp() == testOb->query_max_hp(), 1),

        testOb->heart_beat(),

        assert_equal(testOb->query_hp() <= testOb->query_max_hp(), 1),
    }) :));

    if (r) destruct(r);
}

void test_go () {
    object r1 = new(STD_ROOM);
    object r2 = new(STD_ROOM);

    r1->set_exit("east", base_name(r2));
    r2->set_exit("west", base_name(r1));

    expect("handle_go behaves", (: ({
        // move to object
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(testOb->handle_go($(r2), "go", "east"), 1),

        // move to string path
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(testOb->handle_go(base_name($(r2)), "go", "east"), 1),

        // catch bad move
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_catch((: testOb->handle_go(0, "go", "east") :), "*Bad argument 1 to living->handle_go\n"),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}