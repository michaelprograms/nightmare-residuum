inherit M_TEST;

/**
 * @var {"/daemon/channel"} testOb
 */

string query_cap_name () {
    return "ChannelTest";
}
mapping blocked = ([]);
void toggle_channel_blocked (string channel) {
    blocked[channel] = !blocked[channel];
}
int query_channel_blocked (string channel) {
    return !!blocked[channel];
}

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

void test_listeners () {
    expect("listeners is an array", (: ({
        assert_equal(arrayp(testOb->query_listeners("newbie")), 1),
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

void test_send () {
    object mockChannel = new("/daemon/channel.mock.c");

    mockChannel->start_shadow(testOb);
    expect("channels can send", (: ({
        // bad attempt
        testOb->send("", this_object(), "bad."),
        assert_equal(testOb->query_history(""), UNDEFINED),

        // invalid attempt
        testOb->send("invalid", this_object(), "invalid."),
        assert_equal(testOb->query_history("invalid"), UNDEFINED),

        // valid message
        testOb->send("chat", this_object(), "Test chat."),
        assert_equal(testOb->query_history("chat"), ({ "ChannelTest [[chat]] Test chat." })),

        // invalid message
        testOb->send("connection", this_object(), "Not allowed to send messages on this line."),
        assert_equal(testOb->query_history("connection"), UNDEFINED),

        testOb->send("chat:", this_object(), "chat emotes."),
        assert_equal(testOb->query_history("chat"), ({ "ChannelTest [[chat]] Test chat.", "[[chat]] ChannelTest chat emotes." })),

        testOb->send("chatemote", this_object(), "again."),
        assert_equal(testOb->query_history("chat"), ({ "ChannelTest [[chat]] Test chat.", "[[chat]] ChannelTest chat emotes.", "[[chat]] ChannelTest again." })),

        testOb->send("chat", this_object(), ":again!"),
        assert_equal(testOb->query_history("chat"), ({ "ChannelTest [[chat]] Test chat.", "[[chat]] ChannelTest chat emotes.", "[[chat]] ChannelTest again.", "[[chat]] ChannelTest again!" })),

        testOb->send("chat", this_object(), ": again!"),
        assert_equal(testOb->query_history("chat"), ({ "ChannelTest [[chat]] Test chat.", "[[chat]] ChannelTest chat emotes.", "[[chat]] ChannelTest again.", "[[chat]] ChannelTest again!", "[[chat]] ChannelTest again!" })),
    }) :));

    expect("channels toggle to blocked", (: ({
        // test blocking
        assert_equal(blocked["chat"], UNDEFINED),
        testOb->send("chat", this_object(), 0),
        assert_equal(blocked["chat"], 1),
        testOb->send("chat", this_object(), 0),
        assert_equal(blocked["chat"], 0),
        testOb->send("chat", this_object(), 0),
        assert_equal(blocked["chat"], 1),
        // can send a message when blocked to unblock
        testOb->send("chat", this_object(), "New message."),
        assert_equal(blocked["chat"], 0),
        assert_equal(testOb->query_history("chat")[<1], "ChannelTest [[chat]] New message."),
    }) :));

    mockChannel->stop_shadow();

    if (mockChannel) destruct(mockChannel);
}

void test_send_system () {
    object mockChannel = new("/daemon/channel.mock.c");

    expect("system channels can send", (: ({
        assert_equal($(mockChannel)->start_shadow(testOb), 1),

        testOb->send_system("invalid", "Test invalid."),
        assert_equal(testOb->query_history("invalid"), UNDEFINED),

        testOb->send_system("error", "Test error."),
        assert_equal(testOb->query_history("error"), ({ "((error)) Test error." })),

        testOb->send_system("connection", "Player status change."),
        assert_equal(testOb->query_history("connection"), ({ "((connection)) Player status change." })),

        // @TODO listen for these messages?

        assert_equal($(mockChannel)->stop_shadow(), 1),
    }) :));

    if (mockChannel) destruct(mockChannel);
}