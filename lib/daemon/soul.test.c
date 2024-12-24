inherit M_TEST;

/**
 * @var {"/daemon/soul"} testOb
 */

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
        assert_equal(testOb->add_emote("testemote", "LIV", "$N $vtestemote LIV."), 1),
        assert_equal(testOb->query_emote("testemote"), ([ "": "$N $vtestemote.", "LIV": "$N $vtestemote LIV." ])),

        // can't remove non-existant emote or non-existant rule
        assert_equal(testOb->remove_emote("non-existant", ""), 0),
        assert_equal(testOb->remove_emote("testemote", "non-existant"), 0),
        // remove emote rule
        assert_equal(testOb->remove_emote("testemote", "LIV"), 1),
        // remove emote
        assert_equal(testOb->remove_emote("testemote", ""), 1),
        assert_equal(testOb->query_emote("testemote"), 0),
        // already removed
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