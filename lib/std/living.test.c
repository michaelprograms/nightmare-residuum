inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_living () {
    expect_function("is_living", testOb);

    expect("is_living returns true", (: ({
        assert_equal(testOb->is_living(), 1),
        assert_equal(testOb->is_npc(), 0),
        assert_equal(testOb->is_character(), 0),
    }) :));
}

void test_species () {
    expect_function("query_species", testOb);
    expect_function("set_species", testOb);

    expect("living handles species", (: ({
        assert_equal(testOb->query_species(), "unknown"),
        assert_equal(testOb->set_species("human"), 0),
        assert_equal(testOb->query_species(), "human"),
    }) :));
}

void test_level () {
    expect_function("set_level", testOb);

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