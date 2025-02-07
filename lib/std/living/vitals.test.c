inherit M_TEST;

/**
 * @var {"/std/living/vitals"} testOb
 */

string *test_order () {
    return ({ "test_update_vitals", "test_hp", "test_sp", "test_mp" });
}

void test_update_vitals () {
    expect("update_vitals handles base and max hp/sp/mp", (: ({
        // initial values
        assert_equal(testOb->query_hp(), 0),
        assert_equal(testOb->query_max_hp(), 0),
        assert_equal(testOb->query_sp(), 0),
        assert_equal(testOb->query_max_sp(), 0),
        assert_equal(testOb->query_mp(), 0),
        assert_equal(testOb->query_max_mp(), 0),

        testOb->update_vitals(0),   // don't heal
        assert_equal(testOb->query_hp(), 0),
        assert_equal(testOb->query_max_hp(), 20),
        assert_equal(testOb->query_sp(), 0),
        assert_equal(testOb->query_max_sp(), 10),
        assert_equal(testOb->query_mp(), 0),
        assert_equal(testOb->query_max_mp(), 10),

        testOb->update_vitals(1),   // heal
        assert_equal(testOb->query_hp(), 20),
        assert_equal(testOb->query_hp() == testOb->query_max_hp(), 1),
        assert_equal(testOb->query_sp(), 10),
        assert_equal(testOb->query_sp() == testOb->query_max_sp(), 1),
        assert_equal(testOb->query_mp(), 10),
        assert_equal(testOb->query_mp() == testOb->query_max_mp(), 1),
    }) :));

    expect("null vitals are initialized", (: ({
        assert_equal(testOb->query_hp(), 20),
        assert_equal(testOb->query_max_hp(), 20),
        assert_equal(testOb->query_sp(), 10),
        assert_equal(testOb->query_max_sp(), 10),
        assert_equal(testOb->query_mp(), 10),
        assert_equal(testOb->query_max_mp(), 10),
        store_variable("__Vitals", UNDEFINED, testOb),
        testOb->update_vitals(0),   // don't heal
        assert_equal(testOb->query_hp(), 0),
        assert_equal(testOb->query_max_hp(), 20),
        assert_equal(testOb->query_sp(), 0),
        assert_equal(testOb->query_max_sp(), 10),
        assert_equal(testOb->query_mp(), 0),
        assert_equal(testOb->query_max_mp(), 10),
    }) :));
}

void test_hp () {
    expect("hp handles set/query/query_max", (: ({
        testOb->update_vitals(1),

        testOb->add_hp(1),  // already max
        assert_equal(testOb->query_hp(), 20),
        assert_equal(testOb->query_max_hp(), 20),

        testOb->set_hp(100), // can't go above max
        assert_equal(testOb->query_hp(), 20),

        testOb->add_hp(-10),
        assert_equal(testOb->query_hp(), 10),
        testOb->add_hp(5),
        assert_equal(testOb->query_hp(), 15),
        testOb->add_hp(0),
        assert_equal(testOb->query_hp(), 15),
        testOb->add_hp(10), // attempt to go over max
        assert_equal(testOb->query_hp(), 20),

        testOb->set_hp(5),
        assert_equal(testOb->query_hp(), 5),

        testOb->set_hp(0),
        assert_equal(testOb->query_hp(), 0),
    }) :));
}

void test_sp () {
    expect("sp handles set/query/query_max", (: ({
        testOb->update_vitals(1),

        testOb->add_sp(1),  // already max
        assert_equal(testOb->query_sp(), 10),
        assert_equal(testOb->query_max_sp(), 10),

        testOb->set_sp(100), // can't go above max
        assert_equal(testOb->query_sp(), 10),

        testOb->add_sp(-5),
        assert_equal(testOb->query_sp(), 5),
        testOb->add_sp(2),
        assert_equal(testOb->query_sp(), 7),
        testOb->add_sp(0),
        assert_equal(testOb->query_sp(), 7),
        testOb->add_sp(5), // attempt to go over max
        assert_equal(testOb->query_sp(), 10),

        testOb->set_sp(5),
        assert_equal(testOb->query_sp(), 5),

        testOb->set_sp(0),
        assert_equal(testOb->query_sp(), 0),
    }) :));
}

void test_mp () {
    expect("mp handles set/query/query_max", (: ({
        testOb->update_vitals(1),

        testOb->add_mp(1),  // already max
        assert_equal(testOb->query_mp(), 10),
        assert_equal(testOb->query_max_mp(), 10),

        testOb->set_mp(100), // can't go above max
        assert_equal(testOb->query_mp(), 10),

        testOb->add_mp(-5),
        assert_equal(testOb->query_mp(), 5),
        testOb->add_mp(2),
        assert_equal(testOb->query_mp(), 7),
        testOb->add_mp(0),
        assert_equal(testOb->query_mp(), 7),
        testOb->add_mp(5), // attempt to go over max
        assert_equal(testOb->query_mp(), 10),

        testOb->set_mp(5),
        assert_equal(testOb->query_mp(), 5),

        testOb->set_mp(0),
        assert_equal(testOb->query_mp(), 0),
    }) :));
}