inherit M_TEST;

/**
 * @var {"/daemon/species"} testOb
 */

void test_species_bodies () {
    expect("bodies exist", (: ({
        assert_equal(mapp(testOb->query_bodies()), 1),
        assert_equal(sizeof(testOb->query_bodies()) > 0, 1),
        // verify common bodytype
        assert_equal(mapp(testOb->query_bodies()["quadruped"]), 1),
    }) :));

    expect("species exist", (: ({
        assert_equal(mapp(testOb->query_species()), 1),
        assert_equal(sizeof(testOb->query_species()) > 0, 1),
        // verify common species
        assert_equal(mapp(testOb->query_species()["human"]), 1),
    }) :));
}

void test_setup_body () {
    object ob, npc;
    mapping limbs = ([ ]);

    // setting a species will setup the body
    ob = new(STD_LIVING);
    ob->set_species("human");
    // store the limb data
    foreach (string limb in ob->query_limbs()) {
        limbs[limb] = ob->query_limb(limb);
    }
    npc = new(STD_NPC);
    npc->set_species("invalid");

    expect("species bodies exist", (: ({
        // verify invalid calls are handled
        assert_equal(testOb->setup_body(0), 0),
        assert_equal($(npc)->query_species(), "invalid"),
        assert_equal(testOb->setup_body($(npc)), 0),

        // verify limbs exist
        assert_equal(sizeof(keys($(limbs))) > 0, 1),
        // compare body to limbs
        assert_equal(testOb->setup_body($(ob)), $(limbs)),
    }) :));

    destruct(ob);
    destruct(npc);
}

void test_adjust_stat () {
    expect("stats adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_stat("human", "strength"), 2),

        assert_equal(testOb->query_adjust_stat("unknown species", "strength"), 0),

        assert_equal(testOb->query_adjust_stat("human", "unknown stat"), 0),
    }) :));
}

void test_adjust_vitals () {
    expect("hp adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_hp("human"), 1),

        assert_equal(testOb->query_adjust_hp("nonexistant"), 0),
    }) :));

    expect("sp adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_sp("human"), 1),

        assert_equal(testOb->query_adjust_sp("nonexistant"), 0),
    }) :));

    expect("mp adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_mp("human"), 1),

        assert_equal(testOb->query_adjust_mp("nonexistant"), 0),
    }) :));
}

void test_setup_stats () {
    object ob;

    ob = new(STD_LIVING);
    ob->set_species("human");
    expect("stats are setup", (: ({
        assert_equal($(ob)->query_stat("strength"), 0),
        assert_equal($(ob)->query_stat("perception"), 0),
        assert_equal($(ob)->query_stat("endurance"), 0),
        assert_equal($(ob)->query_stat("charisma"), 0),
        assert_equal($(ob)->query_stat("intelligence"), 0),
        assert_equal($(ob)->query_stat("agility"), 0),
        assert_equal($(ob)->query_stat("luck"), 0),

        testOb->setup_stats($(ob)),

        assert_equal($(ob)->query_stat("strength") >= 5, 1),
        assert_equal($(ob)->query_stat("perception") >= 5, 1),
        assert_equal($(ob)->query_stat("endurance") >= 5, 1),
        assert_equal($(ob)->query_stat("charisma") >= 5, 1),
        assert_equal($(ob)->query_stat("intelligence") >= 5, 1),
        assert_equal($(ob)->query_stat("agility") >= 5, 1),
        assert_equal($(ob)->query_stat("luck") >= 5, 1),

        assert_equal($(ob)->handle_remove(), 1),
    }) :));
}