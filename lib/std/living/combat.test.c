inherit M_TEST;
inherit M_MOVE;
inherit M_LEVELABLE;

/**
 * @var {"/std/living/combat"} testOb
 */

void test_hostiles () {
    object ob1 = new(STD_LIVING), ob2 = new(STD_LIVING);
    object mockCharacter = new("/std/character.mock.c");

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

        // test defeated hostile
        $(ob1)->set_defeated(1),
        assert_equal(testOb->add_hostile($(ob1)), 0),       // can't add defeated hostile
        $(ob1)->set_defeated(0),
        assert_equal(testOb->add_hostile(testOb), 0),       // can't add defeated self

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

int is_living () {
    return 1;
}
void test_parser_applies () {
    expect("combat applies behave", (: ({
        assert_equal(testOb->direct_attack_liv(), 1),
        assert_equal(testOb->direct_aid_liv(), 1),
        testOb->add_hostile(this_object()),
        assert_equal(testOb->direct_attack_liv(), 0),
        assert_equal(testOb->direct_aid_liv(), 0),
    }) :));
}