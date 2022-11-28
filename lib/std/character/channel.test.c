inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/character/channel.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_channels () {
    expect_function("query_channels_available", testOb);
    expect_function("query_channels_blocked", testOb);
    expect_function("query_channel_blocked", testOb);
    expect_function("toggle_channel_blocked", testOb);

    expect("channels handles available and toggling blocked", (: ({
        // default channel status
        assert(testOb->query_channels_available(), "==", ({ "chat", "newbie" })),
        assert(testOb->query_channels_blocked(), "==", ({ })),
        assert(testOb->query_channel_blocked("chat"), "==", 0),
        assert(testOb->query_channel_blocked("nonexistant channel"), "==", 0),
        assert(testOb->query_channel_blocked("connection"), "==", 0),

        // check toggling blocked on
        assert(testOb->toggle_channel_blocked("chat"), "==", 1),
        assert(testOb->query_channel_blocked("chat"), "==", 1),
        assert(testOb->query_channels_available(), "==", ({ "newbie" })),
        assert(testOb->query_channels_blocked(), "==", ({ "chat" })),
        assert(testOb->toggle_channel_blocked("nonexistant channel"), "==", 0),
        assert(testOb->query_channel_blocked("nonexistant channel"), "==", 0),
        assert(testOb->toggle_channel_blocked("connection"), "==", 1),
        assert(testOb->query_channel_blocked("connection"), "==", 1),
        assert(testOb->query_channels_available(), "==", ({ "newbie" })),
        assert(testOb->query_channels_blocked(), "==", ({ "chat", "connection" })),

        // check toggling blocked off
        assert(testOb->toggle_channel_blocked("chat"), "==", 0),
        assert(testOb->query_channel_blocked("chat"), "==", 0),
        assert(testOb->toggle_channel_blocked("connection"), "==", 0),
        assert(testOb->query_channel_blocked("connection"), "==", 0),
        assert(testOb->query_channels_available(), "==", ({ "chat", "newbie" })),
        assert(testOb->query_channels_blocked(), "==", ({ })),
    }) :));
}