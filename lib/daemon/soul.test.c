inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/soul.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_emote () {
    expect_function("query_emote", testOb);
    expect_function("query_emotes", testOb);

    expect("default emotes are queryable", (: ({
        assert(sizeof(testOb->query_emotes()), ">", 0),
        assert(mapp(testOb->query_emote("smile")), "==", 1),
        assert(mapp(testOb->query_emote("frown")), "==", 1),
    }) :));
}

void test_add_and_remove () {
    expect_function("add_emote", testOb);
    expect_function("remove_emote", testOb);

    expect("emotes are addable and removable", (: ({
        // disable save
        testOb->set_save_path(0),
        assert(testOb->query_save_path(), "==", 0),

        assert(!!testOb->query_emote("testemote"), "==", 0),
        assert(testOb->add_emote("testemote", "", ({"$N $vtestemote."})), "==", 1),
        assert(testOb->query_emote("testemote"), "==", ([ "": "$N $vtestemote.", ])),

        assert(testOb->remove_emote("testemote", ""), "==", 1),
        assert(testOb->query_emote("testemote"), "==", 0),
    }) :));

    // @TODO test remove_emote on non-existant emotes/rules
}