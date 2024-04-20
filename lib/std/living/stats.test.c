inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

void test_query_and_set_stat () {
    expect("handles setting and querying stats", (: ({
        testOb->set_stat("strength", 12345),
        testOb->set_stat("perception", 12345),
        testOb->set_stat("endurance", 12345),
        testOb->set_stat("charisma", 12345),
        testOb->set_stat("intelligence", 12345),
        testOb->set_stat("agility", 12345),
        testOb->set_stat("luck", 12345),
        assert_equal(testOb->query_stat("strength"), 12345),
        assert_equal(testOb->query_stat("perception"), 12345),
        assert_equal(testOb->query_stat("endurance"), 12345),
        assert_equal(testOb->query_stat("charisma"), 12345),
        assert_equal(testOb->query_stat("intelligence"), 12345),
        assert_equal(testOb->query_stat("agility"), 12345),
        assert_equal(testOb->query_stat("luck"), 12345),
        assert_equal(testOb->query_stat_base("strength"), 12345),
        assert_equal(testOb->query_stat_base("perception"), 12345),
        assert_equal(testOb->query_stat_base("endurance"), 12345),
        assert_equal(testOb->query_stat_base("charisma"), 12345),
        assert_equal(testOb->query_stat_base("intelligence"), 12345),
        assert_equal(testOb->query_stat_base("agility"), 12345),
        assert_equal(testOb->query_stat_base("luck"), 12345),

        testOb->set_stat("strength", 321),
        testOb->set_stat("perception", 321),
        testOb->set_stat("endurance", 321),
        testOb->set_stat("charisma", 321),
        testOb->set_stat("intelligence", 321),
        testOb->set_stat("agility", 321),
        testOb->set_stat("luck", 321),
        assert_equal(testOb->query_stat("strength"), 321),
        assert_equal(testOb->query_stat("perception"), 321),
        assert_equal(testOb->query_stat("endurance"), 321),
        assert_equal(testOb->query_stat("charisma"), 321),
        assert_equal(testOb->query_stat("intelligence"), 321),
        assert_equal(testOb->query_stat("agility"), 321),
        assert_equal(testOb->query_stat("luck"), 321),
        assert_equal(testOb->query_stat_base("strength"), 321),
        assert_equal(testOb->query_stat_base("perception"), 321),
        assert_equal(testOb->query_stat_base("endurance"), 321),
        assert_equal(testOb->query_stat_base("charisma"), 321),
        assert_equal(testOb->query_stat_base("intelligence"), 321),
        assert_equal(testOb->query_stat_base("agility"), 321),
        assert_equal(testOb->query_stat_base("luck"), 321),
    }) :));
}

void test_stat_bonus () {
    expect("handles setting and querying stats", (: ({
        testOb->set_stat("strength", 100),
        testOb->set_stat("perception", 100),
        testOb->set_stat("endurance", 100),
        testOb->set_stat("charisma", 100),
        testOb->set_stat("intelligence", 100),
        testOb->set_stat("agility", 100),
        testOb->set_stat("luck", 100),
        testOb->add_stat_bonus("strength", 25),
        testOb->add_stat_bonus("perception", 25),
        testOb->add_stat_bonus("endurance", 25),
        testOb->add_stat_bonus("charisma", 25),
        testOb->add_stat_bonus("intelligence", 25),
        testOb->add_stat_bonus("agility", 25),
        testOb->add_stat_bonus("luck", 25),
        assert_equal(testOb->query_stat("strength"), 125),
        assert_equal(testOb->query_stat("perception"), 125),
        assert_equal(testOb->query_stat("endurance"), 125),
        assert_equal(testOb->query_stat("charisma"), 125),
        assert_equal(testOb->query_stat("intelligence"), 125),
        assert_equal(testOb->query_stat("agility"), 125),
        assert_equal(testOb->query_stat("luck"), 125),
        assert_equal(testOb->query_stat_base("strength"), 100),
        assert_equal(testOb->query_stat_base("perception"), 100),
        assert_equal(testOb->query_stat_base("endurance"), 100),
        assert_equal(testOb->query_stat_base("charisma"), 100),
        assert_equal(testOb->query_stat_base("intelligence"), 100),
        assert_equal(testOb->query_stat_base("agility"), 100),
        assert_equal(testOb->query_stat_base("luck"), 100),
        assert_equal(testOb->query_stat_bonus("strength"), 25),
        assert_equal(testOb->query_stat_bonus("perception"), 25),
        assert_equal(testOb->query_stat_bonus("endurance"), 25),
        assert_equal(testOb->query_stat_bonus("charisma"), 25),
        assert_equal(testOb->query_stat_bonus("intelligence"), 25),
        assert_equal(testOb->query_stat_bonus("agility"), 25),
        assert_equal(testOb->query_stat_bonus("luck"), 25),
    }) :));
}