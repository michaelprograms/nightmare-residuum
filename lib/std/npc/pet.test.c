inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/npc/pet.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

string query_key_name () {
    return "/std/npc/pet.test.c";
}

void test_owner () {
    expect_function("set_owner", testOb);
    expect_function("query_owner", testOb);
    expect_function("query_owner_name", testOb);

    expect("owner is settable and queryable", (: ({
        // no owner set
        assert(testOb->query_owner(), "==", UNDEFINED),
        assert(testOb->query_owner_name(), "==", 0),

        testOb->set_owner(this_object()),
        // owner set
        assert(testOb->query_owner(), "==", this_object()),
        assert(testOb->query_owner_name(), "==", "/std/npc/pet.test.c"),
    }) :));
}

void test_following () {
    expect_function("set_following", testOb);
    expect_function("query_following", testOb);

    expect("following is settable and queryable", (: ({
        // not following yet
        assert(testOb->query_following(), "==", UNDEFINED),

        // following
        testOb->set_following(1),
        assert(testOb->query_following(), "==", 1),

        testOb->set_following(0),
        assert(testOb->query_following(), "==", 0),
    }) :));
}