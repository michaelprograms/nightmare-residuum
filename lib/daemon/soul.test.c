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

void test_apply_can_verb_rule () {
    expect_function("can_verb_rule", testOb);

    expect("can_verb_rule is true for valid emotes", (: ({
        assert(testOb->can_verb_rule("smile", ""), "==", 1),
        assert(testOb->can_verb_rule("smile", "LIV"), "==", 1),
        assert(testOb->can_verb_rule("smile", "LVS"), "==", 1),
        assert(testOb->can_verb_rule("frown", ""), "==", 1),
        assert(testOb->can_verb_rule("frown", "LIV"), "==", 1),
        assert(testOb->can_verb_rule("frown", "LVS"), "==", 1),
    }) :));

    expect("can_verb_rule is false for unknown emotes", (: ({
        // valid emote but invalid rule
        assert(testOb->can_verb_rule("smile", "invalid"), "==", 0),
        // invalid emote but valid rule
        assert(testOb->can_verb_rule("invalid", ""), "==", 0),
        // invalid emote and invalid rule
        assert(testOb->can_verb_rule("unknown", "invalid"), "==", 0),
    }) :));

    expect("can_verb_rule is false for invalid arguments", (: ({
        assert(testOb->can_verb_rule(), "==", 0),
        assert(testOb->can_verb_rule("not enough data"), "==", 0),
    }) :));
}