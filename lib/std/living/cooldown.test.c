inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/cooldown.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_cooldown_heart_beats () {
    expect_function("set_cooldown", testOb);
    expect_function("query_cooldown", testOb);
    expect_function("query_cooldowns", testOb);

    expect("heart_beat cooldowns are settable and queryable", (: ({
        // no cooldowns set
        assert(testOb->query_cooldowns(), "==", ([ ])),

        // set a cooldown
        testOb->set_cooldown("test", 1),
        assert(testOb->query_cooldowns(), "==", ([ "test": ([ "type": "heart_beat", "value": 1 ]) ])),
        assert(testOb->query_cooldown("test"), "==", ([ "type": "heart_beat", "value": 1 ])),
        // cooldown expires after one heartbeat
        testOb->heart_beat(),
        assert(testOb->query_cooldowns(), "==", ([ ])),
        assert(testOb->query_cooldown("test"), "==", 0),

        // set a cooldown
        testOb->set_cooldown("test", 2),
        assert(testOb->query_cooldowns(), "==", ([ "test": ([ "type": "heart_beat", "value": 2 ]) ])),
        assert(testOb->query_cooldown("test"), "==", ([ "type": "heart_beat", "value": 2 ])),
        // cooldown ticks down once after one heartbeat
        testOb->heart_beat(),
        assert(testOb->query_cooldowns(), "==", ([ "test": ([ "type": "heart_beat", "value": 1 ]) ])),
        assert(testOb->query_cooldown("test"), "==", ([ "type": "heart_beat", "value": 1 ])),
        // cooldown expires after second heartbeat
        testOb->heart_beat(),
        assert(testOb->query_cooldowns(), "==", ([ ])),
        assert(testOb->query_cooldown("test"), "==", 0),
     }) :));
}

void test_cooldown_timed (function done) {
    expect("timed cooldowns are settable and queryable", (: ({
        // no cooldowns set
        assert(testOb->query_cooldowns(), "==", ([ ])),

        // set a cooldown
        testOb->set_cooldown("test", 0.001, "timed"),
        assert(testOb->query_cooldowns(), "==", ([ "test": ([ "type": "timed", "value": 0.001 ]) ])),
        assert(testOb->query_cooldown("test"), "==", ([ "type": "timed", "value": 0.001 ])),
    }) :));

    call_out_walltime(function (function done) {
        expect("timed cooldowns expire", (: ({
            // cooldown expires after one heartbeat
            assert(testOb->query_cooldowns(), "==", ([ ])),
            assert(testOb->query_cooldown("test"), "==", 0),
        }) :));
        evaluate(done);
    }, 0.002, done);
}