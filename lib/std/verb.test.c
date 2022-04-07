inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/verb.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_requirements () {
    expect_function("set_requirements", testOb);
    expect_function("query_requirements", testOb);
    expect_function("can_verb_rule", testOb);

    expect("requirements should be settable and queryable", (: ({
        assert(testOb->query_requirements(), "==", 0),

        testOb->set_requirements(1),
        assert(testOb->query_requirements(), "==", 1),

        testOb->set_requirements(2),
        assert(testOb->query_requirements(), "==", 2),
    }) :));

    expect("can_verb_rule should use requirements", (: ({
        testOb->set_requirements(0),
        assert(testOb->query_requirements(), "==", 0),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),

        testOb->set_requirements(1),
        assert(testOb->query_requirements(), "==", 1),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),

        testOb->set_requirements(2),
        assert(testOb->query_requirements(), "==", 2),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are not able"),
    }) :));
}