inherit M_TEST;

nosave private int __ImmortalFlag = 0;
int query_immortal () {
    return __ImmortalFlag;
}

void test_paths_and_files () {
    expect("daemon has paths", (: ({
        assert_equal(sizeof(testOb->query_paths()) > 0, 1),
    }) :));
    expect("daemon scanned paths", (: ({
        assert_equal(sizeof(testOb->query_abilities()) > 0, 1),
        assert_equal(sizeof(testOb->query_abilities("/cmd/ability")) > 0, 1),

        assert_equal(sizeof(testOb->query_commands()) > 0, 1),
        assert_equal(sizeof(testOb->query_commands("/cmd/character")) > 0, 1),

        assert_equal(sizeof(testOb->query_verbs()) > 0, 1),
        assert_equal(sizeof(testOb->query_verbs("/cmd/verb")) > 0, 1),
    }) :));
    expect("daemon scanned files", (: ({
        assert_equal(testOb->query_ability("zap"), "/cmd/ability"),
        assert_equal(testOb->query_ability("invalid"), 0),

        assert_equal(testOb->query_command("quit"), "/cmd/character"),
        assert_equal(testOb->query_command("invalid"), 0),

        assert_equal(testOb->query_command("ed"), 0),
        __ImmortalFlag = 1,
        assert_equal(testOb->query_command("ed"), "/secure/cmd/immortal"),
        __ImmortalFlag = 0,

        assert_equal(testOb->query_verb("say"), "/cmd/verb"),
        assert_equal(testOb->query_verb("invalid"), 0),
    }) :));
}