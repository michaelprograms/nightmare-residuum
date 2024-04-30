inherit M_TEST;

void test_channels () {
    expect("channels exist", (: ({
        assert_equal(testOb->query_channels(), ({ "chat", "newbie" })),
    }) :));
    expect("system channels exist", (: ({
        assert_equal(testOb->query_system_channels(), ({ "connection", "error" })),
    }) :));
    expect("channels are validated", (: ({
        assert_equal(testOb->query_valid_channel("chat"), 1),
        assert_equal(testOb->query_valid_channel("chat:"), 1),
        assert_equal(testOb->query_valid_channel("chatemote"), 1),

        assert_equal(testOb->query_valid_channel("connection"), 1),
        assert_equal(testOb->query_valid_channel("connection:"), 1),
        assert_equal(testOb->query_valid_channel("connectionemote"), 1),

        assert_equal(testOb->query_valid_channel("invalid"), 0),
        assert_equal(testOb->query_valid_channel("invalid:"), 0),
        assert_equal(testOb->query_valid_channel("invalidemote"), 0),
    }) :));
}

void test_history () {
    expect("history records", (: ({
        assert_equal(testOb->query_history("invalid"), UNDEFINED),
        assert_equal(testOb->query_history("chat"), UNDEFINED),

        testOb->add_history("chat", "test0"),
        assert_equal(testOb->query_history("chat"), ({ "test0" })),
        testOb->add_history("chat", "test1"),
        testOb->add_history("chat", "test2"),
        testOb->add_history("chat", "test3"),
        testOb->add_history("chat", "test4"),
        testOb->add_history("chat", "test5"),
        testOb->add_history("chat", "test6"),
        testOb->add_history("chat", "test7"),
        testOb->add_history("chat", "test8"),
        testOb->add_history("chat", "test9"),
        testOb->add_history("chat", "test10"),
        testOb->add_history("chat", "test11"),
        testOb->add_history("chat", "test12"),
        testOb->add_history("chat", "test13"),
        testOb->add_history("chat", "test14"),
        testOb->add_history("chat", "test15"),
        testOb->add_history("chat", "test16"),
        testOb->add_history("chat", "test17"),
        testOb->add_history("chat", "test18"),
        testOb->add_history("chat", "test19"),
        assert_equal(testOb->query_history("chat"), ({ "test0", "test1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10", "test11", "test12", "test13", "test14", "test15", "test16", "test17", "test18", "test19" })),

        // history is cycled
        testOb->add_history("chat", "test20"),
        assert_equal(testOb->query_history("chat"), ({ "test1", "test2", "test3", "test4", "test5", "test6", "test7", "test8", "test9", "test10", "test11", "test12", "test13", "test14", "test15", "test16", "test17", "test18", "test19", "test20" })),
    }) :));
}