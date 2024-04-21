inherit M_TEST;

string query_key_name () {
    return "/std/npc/pet.test.c";
}

void test_owner () {
    expect("owner is settable and queryable", (: ({
        // no owner set
        assert_equal(testOb->query_owner(), UNDEFINED),
        assert_equal(testOb->query_owner_name(), 0),

        testOb->set_owner(this_object()),
        // owner set
        assert_equal(testOb->query_owner(), this_object()),
        assert_equal(testOb->query_owner_name(), "/std/npc/pet.test.c"),
    }) :));
}

void test_following () {
    expect("following is settable and queryable", (: ({
        // not following yet
        assert_equal(testOb->query_following(), UNDEFINED),

        // following
        testOb->set_following(1),
        assert_equal(testOb->query_following(), 1),

        testOb->set_following(0),
        assert_equal(testOb->query_following(), 0),
    }) :));
}