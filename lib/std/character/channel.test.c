inherit M_TEST;

/**
 * @var {"/std/character/channel"} testOb
 */

void test_channels () {
    expect("channels handles available and toggling blocked", (: ({
        // default channel status
        assert_equal(testOb->query_channels_available(), ({ "chat", "newbie" })),
        assert_equal(testOb->query_channels_blocked(), ({ })),
        assert_equal(testOb->query_channel_blocked("chat"), 0),
        assert_equal(testOb->query_channel_blocked("nonexistant channel"), 0),
        assert_equal(testOb->query_channel_blocked("connection"), 0),

        // check toggling blocked on
        assert_equal(testOb->toggle_channel_blocked("chat"), 1),
        assert_equal(testOb->query_channel_blocked("chat"), 1),
        assert_equal(testOb->query_channels_available(), ({ "newbie" })),
        assert_equal(testOb->query_channels_blocked(), ({ "chat" })),
        assert_equal(testOb->toggle_channel_blocked("nonexistant channel"), 0),
        assert_equal(testOb->query_channel_blocked("nonexistant channel"), 0),
        assert_equal(testOb->toggle_channel_blocked("connection"), 1),
        assert_equal(testOb->query_channel_blocked("connection"), 1),
        assert_equal(testOb->query_channels_available(), ({ "newbie" })),
        assert_equal(testOb->query_channels_blocked(), ({ "chat", "connection" })),

        // check toggling blocked off
        assert_equal(testOb->toggle_channel_blocked("chat"), 0),
        assert_equal(testOb->query_channel_blocked("chat"), 0),
        assert_equal(testOb->toggle_channel_blocked("connection"), 0),
        assert_equal(testOb->query_channel_blocked("connection"), 0),
        assert_equal(testOb->query_channels_available(), ({ "chat", "newbie" })),
        assert_equal(testOb->query_channels_blocked(), ({ })),
    }) :));

    expect("blocked channels list initialized", (: ({
        assert_equal(testOb->query_channels_blocked(), ({ })),
        assert_equal(testOb->toggle_channel_blocked("chat"), 1),
        assert_equal(testOb->query_channels_blocked(), ({ "chat" })),
        store_variable("__ChannelsBlocked", UNDEFINED, testOb),
        assert_equal(testOb->query_channels_blocked(), ({ })),
    }) :));
}