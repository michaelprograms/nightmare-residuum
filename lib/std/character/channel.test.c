inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/character/channel.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_channels () {
    string *values = ({}), *results = ({});

    expect_function("query_channels_available", testOb);
    expect_function("query_channels_blocked", testOb);
    expect_function("query_channel_blocked", testOb);
    expect_function("toggle_channel_blocked", testOb);

    // default channel status
    values += ({ identify(testOb->query_channels_available()) });
    results += ({ "({ \"chat\" })" });
    values += ({ identify(testOb->query_channels_blocked()) });
    results += ({ "({ })" });
    values += ({ "" + testOb->query_channel_blocked("chat") });
    results += ({ "0" });
    values += ({ "" + testOb->query_channel_blocked("nonexistant channel") });
    results += ({ "0" });
    values += ({ "" + testOb->query_channel_blocked("connection") });
    results += ({ "0" });

    // check toggling blocked on
    values += ({ "" + testOb->toggle_channel_blocked("chat") });
    results += ({ "1" });
    values += ({ "" + testOb->query_channel_blocked("chat") });
    results += ({ "1" });
    values += ({ identify(testOb->query_channels_available()) });
    results += ({ "({ })" });
    values += ({ identify(testOb->query_channels_blocked()) });
    results += ({ "({ \"chat\" })" });
    values += ({ "" + testOb->toggle_channel_blocked("nonexistant channel") });
    results += ({ "0" });
    values += ({ "" + testOb->query_channel_blocked("nonexistant channel") });
    results += ({ "0" });
    values += ({ "" + testOb->toggle_channel_blocked("connection") });
    results += ({ "1" });
    values += ({ "" + testOb->query_channel_blocked("connection") });
    results += ({ "1" });
    values += ({ identify(testOb->query_channels_available()) });
    results += ({ "({ })" });
    values += ({ identify(testOb->query_channels_blocked()) });
    results += ({ "({ \"chat\", \"connection\" })" });

    // check toggling blocked off
    values += ({ "" + testOb->toggle_channel_blocked("chat") });
    results += ({ "0" });
    values += ({ "" + testOb->query_channel_blocked("chat") });
    results += ({ "0" });
    values += ({ "" + testOb->toggle_channel_blocked("connection") });
    results += ({ "0" });
    values += ({ "" + testOb->query_channel_blocked("connection") });
    results += ({ "0" });
    values += ({ identify(testOb->query_channels_available()) });
    results += ({ "({ \"chat\" })" });
    values += ({ identify(testOb->query_channels_blocked()) });
    results += ({ "({ })" });

    expect_arrays_equal(values, results, "channels handled available and blocked");
}