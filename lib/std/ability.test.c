inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/ability.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_name () {
    expect_function("query_name", testOb);

    expect("handles ability name", (: ({
        assert(testOb->query_name(), "==", "ability"),
    }) :));
}

void test_skill_powers () {
    expect_function("query_skill_powers", testOb);
    expect_function("set_skill_powers", testOb);

    expect("handles setting and querying skill powers", (: ({
        assert(testOb->query_skill_powers(), "==", ([ ])),

        testOb->set_skill_powers(([ "brawl": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "brawl": 5, ])),

        testOb->set_skill_powers(([ "ranged": 5, "psionic": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "ranged": 5, "psionic": 5, ])),
    }) :));
}

void test_weapons () {
    expect_function("query_weapons", testOb);
    expect_function("set_weapons", testOb);

    expect("handles setting and querying skill weapons", (: ({
        assert(testOb->query_weapons(), "==", ([ ])),

        testOb->set_weapons(([ "brawl": ({ 1 }), ])),
        assert(testOb->query_weapons(), "==", ([ "brawl": ({ 1 }), ])),

        testOb->set_weapons(([ "brawl": ({ 1 }), "blade": ({ 1, 2 }), "blunt": ({ 1, 2 }), ])),
        assert(testOb->query_weapons(), "==", ([ "brawl": ({ 1 }), "blade": ({ 1, 2 }), "blunt": ({ 1, 2 }), ])),
    }) :));
}

void test_difficulty_factor () {
    expect_function("query_difficulty_factor", testOb);
    expect_function("set_difficulty_factor", testOb);

    expect("handles difficulty factor", (: ({
        // defaults to 100
        assert(testOb->query_difficulty_factor(), "==", 100),

        // check lowering difficulty factor
        testOb->set_difficulty_factor(90),
        assert(testOb->query_difficulty_factor(), "==", 90),

        // check raising difficulty factor
        testOb->set_difficulty_factor(110),
        assert(testOb->query_difficulty_factor(), "==", 110),
    }) :));
}