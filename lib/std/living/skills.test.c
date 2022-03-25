inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/skills.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_all_skills () {
    expect_function("query_all_skills", testOb);

    expect("query_all_skills should return skills", (: ({
        assert(sizeof(testOb->query_all_skills()), "==", 14),
    }) :));
}

void test_query_and_set_skill () {
    expect_function("query_skill", testOb);
    expect_function("set_skill", testOb);

    expect("handles setting and querying skill", (: ({
        testOb->set_skill("melee attack", 12345),
        testOb->set_skill("melee defense", 12345),
        testOb->set_skill("blade attack", 12345),
        testOb->set_skill("blade defense", 12345),
        assert(testOb->query_skill("melee attack"), "==", 12345),
        assert(testOb->query_skill("melee defense"), "==", 12345),
        assert(testOb->query_skill("blade attack"), "==", 12345),
        assert(testOb->query_skill("blade defense"), "==", 12345),
        testOb->set_skill("melee attack", 321),
        testOb->set_skill("melee defense", 321),
        testOb->set_skill("blade attack", 321),
        testOb->set_skill("blade defense", 321),
        assert(testOb->query_skill("melee attack"), "==", 321),
        assert(testOb->query_skill("melee defense"), "==", 321),
        assert(testOb->query_skill("blade attack"), "==", 321),
        assert(testOb->query_skill("blade defense"), "==", 321),
    }) :));
}

void test_train_progress () {
    expect_function("train_skill", testOb);
    expect_function("query_skill_progress", testOb);

    expect("handles training and progress", (: ({
        testOb->set_skill("melee attack", 1),
        assert(testOb->query_skill("melee attack"), "==", 1),
        assert(testOb->query_skill_progress("melee attack"), "==", "0%"),
        assert(to_int(testOb->query_skill_progress("melee attack")), "==", 0),
        testOb->train_skill("melee attack"),
        assert(to_int(testOb->query_skill_progress("melee attack")), ">", 0),
    }) :));
}