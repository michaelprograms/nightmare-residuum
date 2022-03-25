inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/npc.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_npc () {
    expect_function("is_npc", testOb);

    expect("is_npc behaves", (: ({
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_npc(), "==", 1),
        assert(testOb->is_monster(), "==", UNDEFINED),
        assert(testOb->is_character(), "==", UNDEFINED),
    }) :));
}

void test_set_level () {
    expect_function("set_level", testOb);

    expect("set_level adjusts stats and skills", (: ({
        assert(testOb->query_level(), "==", 0),
        assert(testOb->query_stat("strength"), "==", 0),
        assert(testOb->query_stat("endurance"), "==", 0),
        assert(testOb->query_skill("melee attack"), "==", 0),
        assert(testOb->query_skill("melee defense"), "==", 0),

        testOb->set_level(10),
        assert(testOb->query_level(), "==", 10),
        assert(testOb->query_stat("strength"), ">", 0),
        assert(testOb->query_stat("endurance"), ">", 0),
        assert(testOb->query_skill("melee attack"), ">", 0),
        assert(testOb->query_skill("melee defense"), ">", 0),
    }) :));
}