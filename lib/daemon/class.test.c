inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/class.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_adjust_vitals () {
    expect_function("query_adjust_hp", testOb);
    expect_function("query_adjust_sp", testOb);
    expect_function("query_adjust_mp", testOb);

    expect("hp adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_hp("warrior"), 3),
        assert_equal(testOb->query_adjust_hp("mystic"), 2),
        assert_equal(testOb->query_adjust_hp("scoundrel"), 1),
        assert_equal(testOb->query_adjust_hp("ranger"), 0),
        assert_equal(testOb->query_adjust_hp("psionist"), 0),
        assert_equal(testOb->query_adjust_hp("paladin"), 2),

        assert_equal(testOb->query_adjust_hp("nonexistant"), 0),
    }) :));

    expect("sp adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_sp("warrior"), 1),
        assert_equal(testOb->query_adjust_sp("mystic"), 2),
        assert_equal(testOb->query_adjust_sp("scoundrel"), 3),
        assert_equal(testOb->query_adjust_sp("ranger"), 2),
        assert_equal(testOb->query_adjust_sp("psionist"), 1),
        assert_equal(testOb->query_adjust_sp("paladin"), 0),

        assert_equal(testOb->query_adjust_sp("nonexistant"), 0),
    }) :));

    expect("mp adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_mp("warrior"), 0),
        assert_equal(testOb->query_adjust_mp("mystic"), 0),
        assert_equal(testOb->query_adjust_mp("scoundrel"), 0),
        assert_equal(testOb->query_adjust_mp("ranger"), 2),
        assert_equal(testOb->query_adjust_mp("psionist"), 3),
        assert_equal(testOb->query_adjust_mp("paladin"), 2),

        assert_equal(testOb->query_adjust_mp("nonexistant"), 0),
    }) :));
}

void test_query_adjust_stats () {
    expect_function("query_adjust_stat", testOb);

    expect("stat adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_stat("warrior", "strength"), 3),
        assert_equal(testOb->query_adjust_stat("mystic", "strength"), 2),
        assert_equal(testOb->query_adjust_stat("scoundrel", "strength"), 1),
        assert_equal(testOb->query_adjust_stat("ranger", "strength"), 1),
        assert_equal(testOb->query_adjust_stat("psionist", "strength"), 0),
        assert_equal(testOb->query_adjust_stat("paladin", "strength"), 2),

        assert_equal(testOb->query_adjust_stat("nonexistant", "strength"), 0),
        assert_equal(testOb->query_adjust_stat("nonexistant", "nonexistant"), 0),
    }) :));
}

void test_query_max_stats () {
    expect_function("query_max_stat", testOb);

    expect("stat adjustments are queryable", (: ({
        assert_equal(testOb->query_max_stat("warrior", "strength", 1), 5),
        assert_equal(testOb->query_max_stat("mystic", "strength", 1), 4),
        assert_equal(testOb->query_max_stat("scoundrel", "strength", 1), 3),
        assert_equal(testOb->query_max_stat("ranger", "strength", 1), 3),
        assert_equal(testOb->query_max_stat("psionist", "strength", 1), 2),
        assert_equal(testOb->query_max_stat("paladin", "strength", 1), 4),

        assert_equal(testOb->query_max_stat("warrior", "strength", 10), 50),
        assert_equal(testOb->query_max_stat("mystic", "strength", 10), 40),
        assert_equal(testOb->query_max_stat("scoundrel", "strength", 10), 30),
        assert_equal(testOb->query_max_stat("ranger", "strength", 10), 30),
        assert_equal(testOb->query_max_stat("psionist", "strength", 10), 20),
        assert_equal(testOb->query_max_stat("paladin", "strength", 10), 40),

        assert_equal(testOb->query_max_stat("nonexistant", "strength"), 0),
        assert_equal(testOb->query_max_stat("nonexistant", "nonexistant"), 0),
    }) :));
}

void test_query_adjust_skills () {
    expect_function("query_adjust_skill", testOb);

    expect("skill adjustments are queryable", (: ({
        assert_equal(testOb->query_adjust_skill("warrior", "blade attack"), 3),
        assert_equal(testOb->query_adjust_skill("mystic", "blade attack"), 1),
        assert_equal(testOb->query_adjust_skill("scoundrel", "blade attack"), 3),
        assert_equal(testOb->query_adjust_skill("ranger", "blade attack"), 2),
        assert_equal(testOb->query_adjust_skill("psionist", "blade attack"), 0),
        assert_equal(testOb->query_adjust_skill("paladin", "blade attack"), 1),

        assert_equal(testOb->query_adjust_skill("nonexistant", "blade attack"), 0),
        assert_equal(testOb->query_adjust_skill("nonexistant", "nonexistant"), 0),
    }) :));
}