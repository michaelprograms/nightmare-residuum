inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

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