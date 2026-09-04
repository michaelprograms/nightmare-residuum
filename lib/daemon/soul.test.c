inherit M_TEST;

/**
 * @var {"/daemon/soul"} testOb
 */

void test_query_emote() {
    expect("default emotes are queryable", (: ({
        assert_equal(sizeof(testOb->query_emotes()) > 0, 1),
        assert_equal(mapp(testOb->query_emote("smile")), 1),
        assert_equal(mapp(testOb->query_emote("frown")), 1),
    }) :));
}

void test_add_and_remove() {
    expect("emotes are addable and removable", (: ({
        // disable save
        testOb->set_save_path(0),
        assert_equal(testOb->query_save_path(), 0),

        // add dummy emote
        assert_equal(!!testOb->query_emote("testemote"), 0),
        assert_equal(testOb->add_emote("testemote", "", "$N $vtestemote."), 1),
        assert_equal(
            testOb->add_emote("testemote", "LIV", "$N $vtestemote LIV."),
            1
        ),
        assert_equal(
            testOb->query_emote("testemote"),
            ([ "": "$N $vtestemote.", "LIV": "$N $vtestemote LIV." ])
        ),

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

void test_parse_emote() {
    object alice = new(STD_NPC), bob = new(STD_NPC), rock = new(STD_ITEM);

    alice->set_name("alice");
    alice->set_gender("female");
    bob->set_name("bob");
    bob->set_gender("male");
    rock->set_name("rock");

    expect("parse_emote resolves names and pronouns", (: ({
        // $N: the target sees "you", others see the capitalized name
        assert_equal(
            testOb->parse_emote($(alice), "$N", ({ $(alice), $(bob) }), 0),
            "You"
        ),
        assert_equal(
            testOb->parse_emote(0, "$N", ({ $(alice) }), 0),
            "Alice"
        ),
        // repeated reference switches to the subjective pronoun
        assert_equal(
            testOb->parse_emote(0, "$N $N", ({ $(alice) }), 0),
            "Alice She"
        ),
        // $t defaults to index 1 and the objective pronoun on repeat
        assert_equal(
            testOb->parse_emote($(alice), "$t $t", ({ $(alice), $(bob) }), 0),
            "Bob him"
        ),
        // numeric modifiers select the who[] index
        assert_equal(
            testOb->parse_emote(0, "$N0 $N01", ({ $(alice), $(bob) }), 0),
            "Alice Bob"
        ),
    }) :));

    expect("parse_emote handles verbs, object lists and reflexives", (: ({
        // $v pluralizes for third parties, stays singular for the target
        assert_equal(
            testOb->parse_emote($(bob), "$vsmile", ({ $(alice) }), 0),
            "smiles"
        ),
        assert_equal(
            testOb->parse_emote($(alice), "$vsmile", ({ $(alice) }), 0),
            "smile"
        ),
        // $O renders an object list, using "you" for the target
        assert_equal(
            testOb->parse_emote(
                $(alice),
                "$O",
                ({ $(alice) }),
                ({ ({ $(bob), $(rock) }) })
            ),
            "Bob and rock"
        ),
        assert_equal(
            testOb->parse_emote(
                $(bob),
                "$O",
                ({ $(bob) }),
                ({ $(bob) })
            ),
            "You"
        ),
        // reflexive replacement paths still return a string
        assert_equal(
            stringp(testOb->parse_emote(0, "$N $t00", ({ $(alice) }), 0)),
            1
        ),
        assert_equal(
            stringp(
                testOb->parse_emote($(alice), "$N $t00", ({ $(alice) }), 0)
            ),
            1
        ),
        // $R and $P flags execute their branches
        assert_equal(
            stringp(testOb->parse_emote($(alice), "$R", ({ $(alice) }), 0)),
            1
        ),
        assert_equal(
            stringp(testOb->parse_emote(0, "$R", ({ $(alice) }), 0)),
            1
        ),
        assert_equal(
            stringp(testOb->parse_emote($(alice), "$P", ({ $(alice) }), 0)),
            1
        ),
        assert_equal(
            stringp(testOb->parse_emote(0, "$N $P", ({ $(alice) }), 0)),
            1
        ),
        assert_equal(
            stringp(testOb->parse_emote(0, "$P", ({ $(alice) }), 0)),
            1
        ),
        // $Nd and $Np force the name form
        assert_equal(
            testOb->parse_emote(0, "$Nd", ({ $(alice) }), 0),
            "Alice"
        ),
        assert_equal(
            testOb->parse_emote(0, "$Np", ({ $(alice) }), 0),
            "Alice"
        ),
    }) :));

    if (alice) destruct(alice);
    if (bob) destruct(bob);
    if (rock) destruct(rock);
}

void test_apply_can_verb_rule() {
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
