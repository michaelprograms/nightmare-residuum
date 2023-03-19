inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/experience.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

nosave private int __MockLevel, __MockLiving;
int query_level () { return __MockLevel; }
int set_mock_level (int l) { __MockLevel = l; return 1; }
int is_living () { return __MockLiving; }

void test_query_stat_cost () {
    expect_function("query_stat_cost", testOb);

    expect("query_stat_cost returns ascending values", (: ({
        assert(testOb->query_stat_cost("luck", 0, 0, 0), "==", 1),
        assert(testOb->query_stat_cost("luck", -1, 0, 0), "==", 1),
        assert(testOb->query_stat_cost("luck", -50, 0, 0), "==", 1),
        assert(testOb->query_stat_cost("luck", 100, 0, 0), "==", 5052847),
        assert(testOb->query_stat_cost("luck", 1, 0, 0), ">", 1),
        assert(testOb->query_stat_cost("luck", 2, 0, 0), ">", testOb->query_stat_cost("luck", 1, 0, 0)),
        assert(testOb->query_stat_cost("luck", 100, 0, 0), ">", testOb->query_stat_cost("luck", 2, 0, 0)),
        assert(testOb->query_stat_cost("luck", 1000, 0, 0), ">", testOb->query_stat_cost("luck", 100, 0, 0)),
    }) :));

    expect("query_stat_cost adjusted by species and class", (: ({
        assert(testOb->query_stat_cost("luck", 0, "psionist", "human"), "==", 1),
        assert(testOb->query_stat_cost("luck", -1, "psionist", "human"), "==", 1),
        assert(testOb->query_stat_cost("luck", -50, "psionist", "human"), "==", 1),
        assert(testOb->query_stat_cost("luck", 100, "psionist", "human"), "==", 4617050),
        assert(testOb->query_stat_cost("luck", 1, "psionist", "human"), ">", 1),
        assert(testOb->query_stat_cost("luck", 2, "psionist", "human"), ">", testOb->query_stat_cost("luck", 1, "psionist", "human")),
        assert(testOb->query_stat_cost("luck", 100, "psionist", "human"), ">", testOb->query_stat_cost("luck", 2, "psionist", "human")),
        assert(testOb->query_stat_cost("luck", 1000, "psionist", "human"), ">", testOb->query_stat_cost("luck", 100, "psionist", "human")),

        assert(testOb->query_stat_cost("luck", 100, "psionist", "human"), ">", testOb->query_stat_cost("luck", 1, 0, 0)),
    }) :));
}

void test_query_skill_cost () {
    expect_function("query_skill_cost", testOb);

    expect("query_skill_cost returns ascending values", (: ({
        assert(testOb->query_skill_cost("blade attack", 0, 0, 0), "==", 1),
        assert(testOb->query_skill_cost("blade attack", -1, 0, 0), "==", 1),
        assert(testOb->query_skill_cost("blade attack", -50, 0, 0), "==", 1),
        assert(testOb->query_skill_cost("blade attack", 1, 0, 0), ">", 1),
        assert(testOb->query_skill_cost("blade attack", 2, 0, 0), ">", testOb->query_skill_cost("blade attack", 1)),
        assert(testOb->query_skill_cost("blade attack", 100, 0, 0), ">", testOb->query_skill_cost("blade attack", 2)),
        assert(testOb->query_skill_cost("blade attack", 1000, 0, 0), ">", testOb->query_skill_cost("blade attack", 100)),
    }) :));

    // @TODO class and species adjustments
}

void test_query_level_cost () {
    expect_function("query_level_cost", testOb);

    expect("query_level_cost returns ascending values", (: ({
        assert(testOb->query_level_cost(0), "==", 1),
        assert(testOb->query_level_cost(-1), "==", 1),
        assert(testOb->query_level_cost(-50), "==", 1),
        assert(testOb->query_level_cost(1), ">", 1),
        assert(testOb->query_level_cost(2), ">", testOb->query_level_cost(1)),
        assert(testOb->query_level_cost(100), ">", testOb->query_level_cost(2)),
        assert(testOb->query_level_cost(1000), ">", testOb->query_level_cost(100)),
    }) :));
}

void test_query_value () {
    expect_function("query_value", testOb);

    expect("query_value returns ascending values", (: ({
        assert(testOb->query_value(0), "==", 1),
        assert(testOb->query_value(-1), "==", 1),
        assert(testOb->query_value(-50), "==", 1),
        assert(testOb->query_value(1), ">", 1),
        assert(testOb->query_value(2), ">", testOb->query_value(1)),
        assert(testOb->query_value(100), ">", testOb->query_value(2)),
        assert(testOb->query_value(1000), ">", testOb->query_value(100)),
    }) :));

    expect("query_value returns ascending values for integer parameter", (: ({
        assert(testOb->query_value(0), "==", 1),
        assert(testOb->query_value(-1), "==", 1),
        assert(testOb->query_value(-50), "==", 1),
        assert(testOb->query_value(1), ">", 1),
        assert(testOb->query_value(2), ">", testOb->query_value(1)),
        assert(testOb->query_value(100), ">", testOb->query_value(2)),
        assert(testOb->query_value(1000), ">", testOb->query_value(100)),
    }) :));

    __MockLiving = 1;
    expect("query_value returns ascending values for object parameter", (: ({
        set_mock_level(0) && assert(testOb->query_value(this_object()), "==", 1),
        set_mock_level(-1) && assert(testOb->query_value(this_object()), "==", 1),
        set_mock_level(-50) && assert(testOb->query_value(this_object()), "==", 1),
        set_mock_level(1) && assert(testOb->query_value(this_object()), ">", 1),
        set_mock_level(2) && assert(testOb->query_value(this_object()), ">", testOb->query_value(1)),
        set_mock_level(100) && assert(testOb->query_value(this_object()), ">", testOb->query_value(2)),
        set_mock_level(1000) && assert(testOb->query_value(this_object()), ">", testOb->query_value(100)),
    }) :));
    __MockLiving = 0;
}