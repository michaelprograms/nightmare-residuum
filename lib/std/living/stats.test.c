inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/stats.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_and_set_stat () {
    expect_function("query_stat", testOb);
    expect_function("set_stat", testOb);

    expect("handles setting and querying stats", (: ({
        testOb->set_stat("strength", 12345),
        testOb->set_stat("perception", 12345),
        testOb->set_stat("endurance", 12345),
        testOb->set_stat("charisma", 12345),
        testOb->set_stat("intelligence", 12345),
        testOb->set_stat("agility", 12345),
        testOb->set_stat("luck", 12345),
        assert(testOb->query_stat("strength"), "==", 12345),
        assert(testOb->query_stat("perception"), "==", 12345),
        assert(testOb->query_stat("endurance"), "==", 12345),
        assert(testOb->query_stat("charisma"), "==", 12345),
        assert(testOb->query_stat("intelligence"), "==", 12345),
        assert(testOb->query_stat("agility"), "==", 12345),
        assert(testOb->query_stat("luck"), "==", 12345),

        testOb->set_stat("strength", 321),
        testOb->set_stat("perception", 321),
        testOb->set_stat("endurance", 321),
        testOb->set_stat("charisma", 321),
        testOb->set_stat("intelligence", 321),
        testOb->set_stat("agility", 321),
        testOb->set_stat("luck", 321),
        assert(testOb->query_stat("strength"), "==", 321),
        assert(testOb->query_stat("perception"), "==", 321),
        assert(testOb->query_stat("endurance"), "==", 321),
        assert(testOb->query_stat("charisma"), "==", 321),
        assert(testOb->query_stat("intelligence"), "==", 321),
        assert(testOb->query_stat("agility"), "==", 321),
        assert(testOb->query_stat("luck"), "==", 321),
    }) :));
}