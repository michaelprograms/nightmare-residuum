inherit M_TEST;

void test_hostiles () {
    object ob1 = new(STD_LIVING), ob2 = new(STD_LIVING);
    object mockCharacter = new("/std/mock/character.c");

    expect("handles adding, querying, and removing hostiles", (: ({
        assert_equal($(mockCharacter)->start_shadow(testOb), 1),

        // test adding and removing
        assert_equal(sizeof(testOb->query_hostiles()), 0),  // no hostiles
        assert_equal(testOb->add_hostile($(ob1)), 1),       // 1st hostile added
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // 1 hostile
        assert_equal(testOb->query_hostile($(ob1)), 1),     // 1st hostile found
        assert_equal(testOb->add_hostile($(ob1)), 0),       // can't add same hostile
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // still 1 hostile
        assert_equal(testOb->add_hostile($(ob2)), 1),       // 2nd hostile added
        assert_equal(sizeof(testOb->query_hostiles()), 2),  // 2 hostiles
        assert_equal(testOb->query_hostile($(ob2)), 1),     // 2nd hostile found
        assert_equal(testOb->remove_hostile($(ob1)), 1),    // 1st hostile removed
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // 1 hostile
        assert_equal(testOb->remove_hostile($(ob1)), 0),    // can't remove same hostile
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // still 1 hostile
        assert_equal(testOb->remove_hostile($(ob2)), 1),    // 2nd hostile removed
        assert_equal(sizeof(testOb->query_hostiles()), 0),  // 0 hostiles
        assert_equal(testOb->query_hostile($(ob1)), 0),     // 1st hostile not found
        assert_equal(testOb->query_hostile($(ob2)), 0),     // 2nd hostile not found

        // test filtering undefined
        assert_equal(testOb->add_hostile($(ob1)), 1),       // 1st hostile added
        assert_equal(testOb->add_hostile($(ob2)), 1),       // 2nd hostile added
        assert_equal($(ob2)->handle_remove(), 1),           // 2nd hostile destructed
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // only 1 hostile

        assert_equal($(mockCharacter)->stop_shadow(), 1),
    }) :));

    if (ob1) ob1->handle_remove();
    if (ob2) ob2->handle_remove();
    if (mockCharacter) destruct(mockCharacter);
}