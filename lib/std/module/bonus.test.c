inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/bonus.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_singular_bonus () {
    expect_function("query_bonus", testOb);
    expect_function("set_bonus", testOb);

    expect("handles setting and querying bonus", (: ({
        assert(testOb->query_bonus("strength"), "==", 0),
        assert(testOb->query_bonus("luck"), "==", 0),

        assert(testOb->set_bonus("strength", 1), "==", 1),
        assert(testOb->query_bonus("strength"), "==", 1),

        assert(testOb->set_bonus("strength", 2), "==", 2),
        assert(testOb->query_bonus("strength"), "==", 2),

        assert(testOb->set_bonus("luck", 3), "==", 3),
        assert(testOb->query_bonus("luck"), "==", 3),
    }) :));
}

void test_multiple_bonuses () {
    expect_function("query_bonuses", testOb);
    expect_function("set_bonuses", testOb);

    expect("handles setting and querying bonuses", (: ({
        assert(testOb->query_bonuses(), "==", ([ ])),

        assert(testOb->set_bonuses(([ "strength": 1 ])), "==", ([ "strength": 1, ])),
        assert(testOb->query_bonuses(), "==", ([ "strength": 1, ])),

        assert(testOb->set_bonuses(([ "strength": 2 ])), "==", ([ "strength": 2, ])),
        assert(testOb->query_bonuses(), "==", ([ "strength": 2, ])),

        assert(testOb->set_bonuses(([ "luck": 3 ])), "==", ([ "strength": 2, "luck": 3, ])),
        assert(testOb->query_bonuses(), "==", ([ "strength": 2, "luck": 3, ])),
    }) :));
}