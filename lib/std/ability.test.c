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

void test_skill_type () {
    expect_function("query_skill_type", testOb);
    expect_function("set_skill_type", testOb);

    expect("handles skill type", (: ({
        assert(testOb->query_skill_type(), "==", 0),
        testOb->set_skill_type("melee"),
        assert(testOb->query_skill_type(), "==", "melee"),
    }) :));
}

void test_base_power () {
    expect_function("query_base_power", testOb);
    expect_function("set_base_power", testOb);

    expect("handles skill type", (: ({
        assert(testOb->query_base_power(), "==", 0),
        testOb->set_base_power(123),
        assert(testOb->query_base_power(), "==", 123),
    }) :));
}