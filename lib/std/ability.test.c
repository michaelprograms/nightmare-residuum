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

    expect("handles skill powers", (: ({
        assert(testOb->query_skill_powers(), "==", ([ ])),

        testOb->set_skill_powers(([ "melee": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "melee": 5, ])),

        testOb->set_skill_powers(([ "ranged": 5, "psionic": 5, ])),
        assert(testOb->query_skill_powers(), "==", ([ "ranged": 5, "psionic": 5, ])),
    }) :));

    expect("set_skill_powers handles invalid argument 1", (: ({
        assert((: testOb->set_skill_powers(this_object()) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers(1) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers(1.0) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers("") :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers(({})) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
        assert((: testOb->set_skill_powers((: 1 :)) :), "catch", "*Bad argument 1 to ability->set_skill_powers\n"),
    }) :));
}