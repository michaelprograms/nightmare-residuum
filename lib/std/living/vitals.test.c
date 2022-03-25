inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living/vitals.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_update_vitals", "test_hp", "test_sp", "test_mp" });
}

void test_update_vitals () {
    expect_function("update_vitals", testOb);

    expect("update_vitals handles base and max hp/sp/mp", (: ({
        // initial values
        assert(testOb->query_hp(), "==", 0),
        assert(testOb->query_max_hp(), "==", 0),
        assert(testOb->query_sp(), "==", 0),
        assert(testOb->query_max_sp(), "==", 0),
        assert(testOb->query_mp(), "==", 0),
        assert(testOb->query_max_mp(), "==", 0),

        testOb->update_vitals(0),   // don't heal
        assert(testOb->query_hp(), "==", 0),
        assert(testOb->query_max_hp(), "==", 20),
        assert(testOb->query_sp(), "==", 0),
        assert(testOb->query_max_sp(), "==", 10),
        assert(testOb->query_mp(), "==", 0),
        assert(testOb->query_max_mp(), "==", 10),

        testOb->update_vitals(1),   // heal
        assert(testOb->query_hp(), "==", 20),
        assert(testOb->query_hp() == testOb->query_max_hp(), "==", 1),
        assert(testOb->query_sp(), "==", 10),
        assert(testOb->query_sp() == testOb->query_max_sp(), "==", 1),
        assert(testOb->query_mp(), "==", 10),
        assert(testOb->query_mp() == testOb->query_max_mp(), "==", 1),
    }) :));
}

void test_hp () {
    expect_function("set_hp", testOb);
    expect_function("add_hp", testOb);
    expect_function("query_hp", testOb);
    expect_function("query_max_hp", testOb);

    expect("hp handles set/query/query_max", (: ({
        testOb->add_hp(1),  // already max
        assert(testOb->query_hp(), "==", 20),

        testOb->add_hp(-10),
        assert(testOb->query_hp(), "==", 10),
        testOb->add_hp(5),
        assert(testOb->query_hp(), "==", 15),
        testOb->add_hp(0),
        assert(testOb->query_hp(), "==", 15),
        testOb->add_hp(10), // attempt to go over max
        assert(testOb->query_hp(), "==", 20),

        testOb->set_hp(5),
        assert(testOb->query_hp(), "==", 5),

        testOb->set_hp(0),
        assert(testOb->query_hp(), "==", 0),
    }) :));
}

void test_sp () {
    expect_function("set_sp", testOb);
    expect_function("add_sp", testOb);
    expect_function("query_sp", testOb);
    expect_function("query_max_sp", testOb);

    expect("sp handles set/query/query_max", (: ({
        testOb->add_sp(1),  // already max
        assert(testOb->query_sp(), "==", 10),

        testOb->add_sp(-5),
        assert(testOb->query_sp(), "==", 5),
        testOb->add_sp(2),
        assert(testOb->query_sp(), "==", 7),
        testOb->add_sp(0),
        assert(testOb->query_sp(), "==", 7),
        testOb->add_sp(5), // attespt to go over max
        assert(testOb->query_sp(), "==", 10),

        testOb->set_sp(5),
        assert(testOb->query_sp(), "==", 5),

        testOb->set_sp(0),
        assert(testOb->query_sp(), "==", 0),
    }) :));
}

void test_mp () {
    expect_function("set_mp", testOb);
    expect_function("add_mp", testOb);
    expect_function("query_mp", testOb);
    expect_function("query_max_mp", testOb);

    expect("mp handles set/query/query_max", (: ({
        testOb->add_mp(1),  // already max
        assert(testOb->query_mp(), "==", 10),

        testOb->add_mp(-5),
        assert(testOb->query_mp(), "==", 5),
        testOb->add_mp(2),
        assert(testOb->query_mp(), "==", 7),
        testOb->add_mp(0),
        assert(testOb->query_mp(), "==", 7),
        testOb->add_mp(5), // attempt to go over max
        assert(testOb->query_mp(), "==", 10),

        testOb->set_mp(5),
        assert(testOb->query_mp(), "==", 5),

        testOb->set_mp(0),
        assert(testOb->query_mp(), "==", 0),
    }) :));
}