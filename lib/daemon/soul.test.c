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

void test_query_emote () {
    expect("default emotes are queryable", (: ({
        assert_equal(sizeof(testOb->query_emotes()) > 0, 1),
        assert_equal(mapp(testOb->query_emote("smile")), 1),
        assert_equal(mapp(testOb->query_emote("frown")), 1),
    }) :));
}

void test_add_and_remove () {
    expect("emotes are addable and removable", (: ({
        // disable save
        testOb->set_save_path(0),
        assert_equal(testOb->query_save_path(), 0),

        // add dummy emote
        assert_equal(!!testOb->query_emote("testemote"), 0),
        assert_equal(testOb->add_emote("testemote", "", "$N $vtestemote."), 1),
        assert_equal(testOb->query_emote("testemote"), ([ "": "$N $vtestemote.", ])),

        // remove emote
        assert_equal(testOb->remove_emote("testemote", ""), 1),
        assert_equal(testOb->query_emote("testemote"), 0),
        // can't remove again
        assert_equal(testOb->remove_emote("testemote", ""), 0),
    }) :));
}

void test_apply_can_verb_rule () {
    expect("can_verb_rule is true for valid emotes", (: ({
        assert_equal(testOb->can_verb_rule("smile", ""), 1),
        assert_equal(testOb->can_verb_rule("smile", "LIV"), 1),
        assert_equal(testOb->can_verb_rule("smile", "LVS"), 1),
        assert_equal(testOb->can_verb_rule("frown", ""), 1),
        assert_equal(testOb->can_verb_rule("frown", "LIV"), 1),
        assert_equal(testOb->can_verb_rule("frown", "LVS"), 1),
    }) :));

    expect("can_verb_rule is false for unknown emotes", (: ({
        // valid emote but invalid rule
        assert_equal(testOb->can_verb_rule("smile", "invalid"), 0),
        // invalid emote but valid rule
        assert_equal(testOb->can_verb_rule("invalid", ""), 0),
        // invalid emote and invalid rule
        assert_equal(testOb->can_verb_rule("unknown", "invalid"), 0),
    }) :));

    expect("can_verb_rule is false for invalid arguments", (: ({
        assert_equal(testOb->can_verb_rule(), 0),
        assert_equal(testOb->can_verb_rule("not enough data"), 0),
    }) :));
}