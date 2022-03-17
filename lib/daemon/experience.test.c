inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/daemon/experience.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_ignore () {
    return ::test_ignore() + ({ "query_level", "set_mock_level", "is_living" });
}

nosave private int __MockLevel, __MockLiving;
int query_level () { return __MockLevel; }
int set_mock_level (int l) { __MockLevel = l; return 1; }
int is_living () { return __MockLiving; }

void test_query_stat_cost () {
    expect_function("query_stat_cost", testOb);

    expect("query_stat_cost returns ascending values", (: ({
        assert((: testOb->query_stat_cost("luck", 0) :), "==", 0),
        assert((: testOb->query_stat_cost("luck", -1) :), "==", 0),
        assert((: testOb->query_stat_cost("luck", -50) :), "==", 0),
        assert((: testOb->query_stat_cost("luck", 1) :), ">", 0),
        assert((: testOb->query_stat_cost("luck", 2) :), ">", (: testOb->query_stat_cost("luck", 1) :)),
        assert((: testOb->query_stat_cost("luck", 100) :), ">", (: testOb->query_stat_cost("luck", 2) :)),
        assert((: testOb->query_stat_cost("luck", 1000) :), ">", (: testOb->query_stat_cost("luck", 100) :)),
    }) :));

    // check error conditions
    expect_catches (({
        (: testOb->query_stat_cost(this_object()) :),
        (: testOb->query_stat_cost(1) :),
        (: testOb->query_stat_cost(1.0) :),
        (: testOb->query_stat_cost(({})) :),
        (: testOb->query_stat_cost(([])) :),
        (: testOb->query_stat_cost((: 1 :)) :),
    }), "*Bad argument 1 to experience->query_stat_cost\n", "query_stat_cost handled invalid argument 1");
    expect_catches (({
        (: testOb->query_stat_cost("strength", this_object()) :),
        (: testOb->query_stat_cost("strength", 1.0) :),
        (: testOb->query_stat_cost("strength", "") :),
        (: testOb->query_stat_cost("strength", ({})) :),
        (: testOb->query_stat_cost("strength", ([])) :),
        (: testOb->query_stat_cost("strength", (: 1 :)) :),
    }), "*Bad argument 2 to experience->query_stat_cost\n", "query_stat_cost handled invalid argument 2");
}

void test_query_skill_cost () {
    expect_function("query_skill_cost", testOb);

    expect("query_skill_cost returns ascending values", (: ({
        assert((: testOb->query_skill_cost(0) :), "==", 0),
        assert((: testOb->query_skill_cost(-1) :), "==", 0),
        assert((: testOb->query_skill_cost(-50) :), "==", 0),
        assert((: testOb->query_skill_cost(1) :), ">", 0),
        assert((: testOb->query_skill_cost(2) :), ">", (: testOb->query_skill_cost(1) :)),
        assert((: testOb->query_skill_cost(100) :), ">", (: testOb->query_skill_cost(2) :)),
        assert((: testOb->query_skill_cost(1000) :), ">", (: testOb->query_skill_cost(100) :)),
    }) :));

    // check error conditions
    expect_catches (({
        (: testOb->query_skill_cost(this_object()) :),
        (: testOb->query_skill_cost("") :),
        (: testOb->query_skill_cost(1.0) :),
        (: testOb->query_skill_cost(({})) :),
        (: testOb->query_skill_cost(([])) :),
        (: testOb->query_skill_cost((: 1 :)) :),
    }), "*Bad argument 1 to experience->query_skill_cost\n", "query_skill_cost handled invalid argument 1");
}

void test_query_value () {
    mixed *values = ({}), *results = ({});

    expect_function("query_value", testOb);

    expect("query_value returns ascending values", (: ({
        assert((: testOb->query_value(0) :), "==", 0),
        assert((: testOb->query_value(-1) :), "==", 0),
        assert((: testOb->query_value(-50) :), "==", 0),
        assert((: testOb->query_value(1) :), ">", 0),
        assert((: testOb->query_value(2) :), ">", (: testOb->query_value(1) :)),
        assert((: testOb->query_value(100) :), ">", (: testOb->query_value(2) :)),
        assert((: testOb->query_value(1000) :), ">", (: testOb->query_value(100) :)),
    }) :));

    // tests for integer parameter
    expect("query_value returns ascending values for integer parameter", (: ({
        assert((: testOb->query_value(0) :), "==", 0),
        assert((: testOb->query_value(-1) :), "==", 0),
        assert((: testOb->query_value(-50) :), "==", 0),
        assert((: testOb->query_value(1) :), ">", 0),
        assert((: testOb->query_value(2) :), ">", (: testOb->query_value(1) :)),
        assert((: testOb->query_value(100) :), ">", (: testOb->query_value(2) :)),
        assert((: testOb->query_value(1000) :), ">", (: testOb->query_value(100) :)),
    }) :));

    // tests for object parameter
    __MockLiving = 1;
    expect("query_value returns ascending values for object parameter", (: ({
        set_mock_level(0) && assert((: testOb->query_value(this_object()) :), "==", 0),
        set_mock_level(-1) && assert((: testOb->query_value(this_object()) :), "==", 0),
        set_mock_level(-50) && assert((: testOb->query_value(this_object()) :), "==", 0),
        set_mock_level(1) && assert((: testOb->query_value(this_object()) :), ">", 0),
        set_mock_level(2) && assert((: testOb->query_value(this_object()) :), ">", (: testOb->query_value(1) :)),
        set_mock_level(100) && assert((: testOb->query_value(this_object()) :), ">", (: testOb->query_value(2) :)),
        set_mock_level(1000) && assert((: testOb->query_value(this_object()) :), ">", (: testOb->query_value(100) :)),
    }) :));

    // check error conditions
    __MockLiving = 0;
    expect_catches (({
        (: testOb->query_value(this_object()) :),
        (: testOb->query_value(1.0) :),
        (: testOb->query_value("") :),
        (: testOb->query_value(({})) :),
        (: testOb->query_value(([])) :),
        (: testOb->query_value((: 1 :)) :),
    }), "*Bad argument 1 to experience->query_value\n", "query_value handled invalid argument 1");
}