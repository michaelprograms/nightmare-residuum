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
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_npc(), "==", 0),
        assert(testOb->is_character(), "==", 0),
    }) :));
}

void test_species () {
    expect_function("query_species", testOb);
    expect_function("set_species", testOb);

    expect("living handles species", (: ({
        assert(testOb->query_species(), "==", "unknown"),
        assert(testOb->set_species("human"), "==", 0),
        assert(testOb->query_species(), "==", "human"),
    }) :));
}

void test_level () {
    expect_function("set_level", testOb);

    // @TODO check limbs max damage
    expect("level updates vitals", (: ({
        testOb->set_species("human"),
        assert(testOb->query_species(), "==", "human"),
        assert(testOb->query_level(), "==", 1),
        assert(testOb->query_hp(), "==", 22),
        assert(testOb->query_sp(), "==", 12),
        assert(testOb->query_mp(), "==", 12),
        assert(testOb->query_max_hp(), "==", 22),
        assert(testOb->query_max_sp(), "==", 12),
        assert(testOb->query_max_mp(), "==", 12),

        // adjust HP
        testOb->set_hp(1),
        testOb->set_sp(1),
        testOb->set_mp(1),
        assert(testOb->query_hp(), "==", 1),
        assert(testOb->query_sp(), "==", 1),
        assert(testOb->query_mp(), "==", 1),

        // advance level
        testOb->set_level(2),
        // vitals are updated and healed
        assert(testOb->query_hp(), "==", 33),
        assert(testOb->query_sp(), "==", 18),
        assert(testOb->query_mp(), "==", 18),
        assert(testOb->query_max_hp(), "==", 33),
        assert(testOb->query_max_sp(), "==", 18),
        assert(testOb->query_max_mp(), "==", 18),
    }) :));
}