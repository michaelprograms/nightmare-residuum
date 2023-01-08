inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/combat.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_hostiles () {
    object ob1 = new(STD_LIVING), ob2 = new(STD_LIVING);
    object mockCharacter = new("/std/mock/character.c");

    expect_function("add_hostile", testOb);
    expect_function("remove_hostile", testOb);
    expect_function("query_hostile", testOb);
    expect_function("query_hostiles", testOb);

    expect("handles adding, querying, and removing hostiles", (: ({
        assert($(mockCharacter)->start_shadow(testOb), "==", 1),

        // test adding and removing
        assert(sizeof(testOb->query_hostiles()), "==", 0),  // no hostiles
        assert(testOb->add_hostile($(ob1)), "==", 1),       // 1st hostile added
        assert(sizeof(testOb->query_hostiles()), "==", 1),  // 1 hostile
        assert(testOb->query_hostile($(ob1)), "==", 1),     // 1st hostile found
        assert(testOb->add_hostile($(ob1)), "==", 0),       // can't add same hostile
        assert(sizeof(testOb->query_hostiles()), "==", 1),  // still 1 hostile
        assert(testOb->add_hostile($(ob2)), "==", 1),       // 2nd hostile added
        assert(sizeof(testOb->query_hostiles()), "==", 2),  // 2 hostiles
        assert(testOb->query_hostile($(ob2)), "==", 1),     // 2nd hostile found
        assert(testOb->remove_hostile($(ob1)), "==", 1),    // 1st hostile removed
        assert(sizeof(testOb->query_hostiles()), "==", 1),  // 1 hostile
        assert(testOb->remove_hostile($(ob1)), "==", 0),    // can't remove same hostile
        assert(sizeof(testOb->query_hostiles()), "==", 1),  // still 1 hostile
        assert(testOb->remove_hostile($(ob2)), "==", 1),    // 2nd hostile removed
        assert(sizeof(testOb->query_hostiles()), "==", 0),  // 0 hostiles
        assert(testOb->query_hostile($(ob1)), "==", 0),     // 1st hostile not found
        assert(testOb->query_hostile($(ob2)), "==", 0),     // 2nd hostile not found

        // test filtering undefined
        assert(testOb->add_hostile($(ob1)), "==", 1),       // 1st hostile added
        assert(testOb->add_hostile($(ob2)), "==", 1),       // 2nd hostile added
        assert($(ob2)->handle_remove(), "==", 1),           // 2nd hostile destructed
        assert(sizeof(testOb->query_hostiles()), "==", 1),  // only 1 hostile

        assert($(mockCharacter)->stop_shadow(), "==", 1),
    }) :));

    if (ob1) ob1->handle_remove();
    if (ob2) ob2->handle_remove();
    if (mockCharacter) destruct(mockCharacter);
}