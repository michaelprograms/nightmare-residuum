inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/daemon/experience.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_ignore () {
    return ::test_ignore() + ({ "query_level", "is_living" });
}

nosave private int __MockLevel, __MockLiving;
int query_level () { return __MockLevel; }
int is_living () { return __MockLiving; }

void test_query_value () {
    mixed *values = ({}), *results = ({});

    expect_function("query_value", testOb);

    values += ({ testOb->query_value(0) });
    results += ({ 0 });
    values += ({ testOb->query_value(-1) });
    results += ({ 0 });
    values += ({ testOb->query_value(-50) });
    results += ({ 0 });
    values += ({ testOb->query_value(1) > 0 });
    results += ({ 1 });
    values += ({ testOb->query_value(2) > values[<1] });
    results += ({ 1 });
    values += ({ testOb->query_value(100) > values[<1] });
    results += ({ 1 });
    values += ({ testOb->query_value(1000) > values[<1] });
    results += ({ 1 });

    // repeat tests for object
    __MockLiving = 1;
    __MockLevel = 0;
    values += ({ testOb->query_value(this_object()) });
    results += ({ 0 });
    __MockLevel = -1;
    values += ({ testOb->query_value(this_object()) });
    results += ({ 0 });
    __MockLevel = -50;
    values += ({ testOb->query_value(this_object()) });
    results += ({ 0 });
    __MockLevel = 1;
    values += ({ testOb->query_value(this_object()) > 0 });
    results += ({ 1 });
    __MockLevel = 2;
    values += ({ testOb->query_value(this_object()) > values[<1] });
    results += ({ 1 });
    values += ({ testOb->query_value(this_object()) > values[<1] });
    results += ({ 1 });
    values += ({ testOb->query_value(this_object()) > values[<1] });
    results += ({ 1 });

    expect_arrays_equal(values, results, "query_value returned ascending values");

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

void test_query_stat_cost () {
    mixed *values = ({}), *results = ({});

    expect_function("query_stat_cost", testOb);

    values += ({ testOb->query_stat_cost("strength", 0) });
    results += ({ 0 });
    values += ({ testOb->query_stat_cost("strength", -1) });
    results += ({ 0 });
    values += ({ testOb->query_stat_cost("strength", -50) });
    results += ({ 0 });
    values += ({ testOb->query_stat_cost("strength", 1) > 0 });
    results += ({ 1 });
    values += ({ testOb->query_stat_cost("strength", 2) > values[<1] });
    results += ({ 1 });
    values += ({ testOb->query_stat_cost("strength", 100) > values[<1] });
    results += ({ 1 });
    values += ({ testOb->query_stat_cost("strength", 1000) > values[<1] });
    results += ({ 1 });

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