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
        assert(testOb->query_stat_cost("luck", 0), "==", 1),
        assert(testOb->query_stat_cost("luck", -1), "==", 1),
        assert(testOb->query_stat_cost("luck", -50), "==", 1),
        assert(testOb->query_stat_cost("luck", 1), ">", 1),
        assert(testOb->query_stat_cost("luck", 2), ">", testOb->query_stat_cost("luck", 1)),
        assert(testOb->query_stat_cost("luck", 100), ">", testOb->query_stat_cost("luck", 2)),
        assert(testOb->query_stat_cost("luck", 1000), ">", testOb->query_stat_cost("luck", 100)),
    }) :));

    expect("query_stat_cost handles invalid argument 1", (: ({
        assert((: testOb->query_stat_cost(this_object()) :), "catch", "*Bad argument 1 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost(1) :), "catch", "*Bad argument 1 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost(1.0) :), "catch", "*Bad argument 1 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost(({})) :), "catch", "*Bad argument 1 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost(([])) :), "catch", "*Bad argument 1 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost((: 1 :)) :), "catch", "*Bad argument 1 to experience->query_stat_cost\n"),
    }) :));
    expect("query_stat_cost handles invalid argument 2", (: ({
        assert((: testOb->query_stat_cost("strength", this_object()) :), "catch", "*Bad argument 2 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost("strength", 1.0) :), "catch", "*Bad argument 2 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost("strength", "") :), "catch", "*Bad argument 2 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost("strength", ({})) :), "catch", "*Bad argument 2 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost("strength", ([])) :), "catch", "*Bad argument 2 to experience->query_stat_cost\n"),
        assert((: testOb->query_stat_cost("strength", (: 1 :)) :), "catch", "*Bad argument 2 to experience->query_stat_cost\n"),
    }) :));
}

void test_query_skill_cost () {
    expect_function("query_skill_cost", testOb);

    expect("query_skill_cost returns ascending values", (: ({
        assert(testOb->query_skill_cost(0), "==", 1),
        assert(testOb->query_skill_cost(-1), "==", 1),
        assert(testOb->query_skill_cost(-50), "==", 1),
        assert(testOb->query_skill_cost(1), ">", 1),
        assert(testOb->query_skill_cost(2), ">", testOb->query_skill_cost(1)),
        assert(testOb->query_skill_cost(100), ">", testOb->query_skill_cost(2)),
        assert(testOb->query_skill_cost(1000), ">", testOb->query_skill_cost(100)),
    }) :));

    expect("query_skill_cost handles invalid argument 1", (: ({
        assert((: testOb->query_skill_cost(this_object()) :), "catch", "*Bad argument 1 to experience->query_skill_cost\n"),
        assert((: testOb->query_skill_cost("") :), "catch", "*Bad argument 1 to experience->query_skill_cost\n"),
        assert((: testOb->query_skill_cost(1.0) :), "catch", "*Bad argument 1 to experience->query_skill_cost\n"),
        assert((: testOb->query_skill_cost(({})) :), "catch", "*Bad argument 1 to experience->query_skill_cost\n"),
        assert((: testOb->query_skill_cost(([])) :), "catch", "*Bad argument 1 to experience->query_skill_cost\n"),
        assert((: testOb->query_skill_cost((: 1 :)) :), "catch", "*Bad argument 1 to experience->query_skill_cost\n"),
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
    expect("query_value handled invalid argument 1", (: ({
        assert((: testOb->query_value(this_object()) :), "catch", "*Bad argument 1 to experience->query_value\n"),
        assert((: testOb->query_value(1.0) :), "catch", "*Bad argument 1 to experience->query_value\n"),
        assert((: testOb->query_value("") :), "catch", "*Bad argument 1 to experience->query_value\n"),
        assert((: testOb->query_value(({})) :), "catch", "*Bad argument 1 to experience->query_value\n"),
        assert((: testOb->query_value(([])) :), "catch", "*Bad argument 1 to experience->query_value\n"),
        assert((: testOb->query_value((: 1 :)) :), "catch", "*Bad argument 1 to experience->query_value\n"),
    }) :));
}