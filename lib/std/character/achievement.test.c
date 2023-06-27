inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/character/achievement.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_achievements () {
    expect_function("set_achievement", testOb);
    expect_function("query_achievement", testOb);
    expect_function("query_achievements", testOb);

    expect("achievements are settable and queryable", (: ({
        assert(testOb->query_achievements(), "==", ({ })),

        assert(testOb->set_achievement("test1"), "==", 1),
        assert(testOb->query_achievement("test1"), "==", 1),
        assert(testOb->set_achievement("test1"), "==", 0), // already set
        assert(testOb->query_achievement("test1"), "==", 1),

        assert(testOb->set_achievement("test2"), "==", 1),
        assert(testOb->query_achievement("test2"), "==", 1),
        assert(testOb->set_achievement("test2"), "==", 0), // already set
        assert(testOb->query_achievement("test2"), "==", 1),

        assert(testOb->query_achievements(), "==", ({ "test2", "test1", })),
    }) :));
}