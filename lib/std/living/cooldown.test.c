inherit M_TEST;

void test_cooldown_heart_beats () {
    expect("null attributes are initialized", (: ({
        store_variable("__Cooldown", UNDEFINED, testOb),
        assert_equal(testOb->query_cooldowns(), ([ ])),
        assert_equal(testOb->query_cooldown("test"), 0),
    }) :));

    expect("heart_beat cooldowns are settable and queryable", (: ({
        // no cooldowns set
        assert_equal(testOb->query_cooldowns(), ([ ])),
        assert_equal(testOb->query_cooldown("test"), 0),

        // set a cooldown
        testOb->set_cooldown("test", 1),
        assert_equal(testOb->query_cooldowns(), ([ "test": ([ "type": "heart_beat", "value": 1 ]) ])),
        assert_equal(testOb->query_cooldown("test"), 1),
        // cooldown expires after one heartbeat
        testOb->heart_beat(),
        assert_equal(testOb->query_cooldowns(), ([ ])),
        assert_equal(testOb->query_cooldown("test"), 0),

        // set a cooldown
        testOb->set_cooldown("test", 2),
        assert_equal(testOb->query_cooldowns(), ([ "test": ([ "type": "heart_beat", "value": 2 ]) ])),
        assert_equal(testOb->query_cooldown("test"), 2),
        // cooldown ticks down once after one heartbeat
        testOb->heart_beat(),
        assert_equal(testOb->query_cooldowns(), ([ "test": ([ "type": "heart_beat", "value": 1 ]) ])),
        assert_equal(testOb->query_cooldown("test"), 1),
        // cooldown expires after second heartbeat
        testOb->heart_beat(),
        assert_equal(testOb->query_cooldowns(), ([ ])),
        assert_equal(testOb->query_cooldown("test"), 0),
     }) :));
}

void test_cooldown_timed (function done) {
    expect("timed cooldowns are settable and queryable", (: ({
        // no cooldowns set
        assert_equal(testOb->query_cooldowns(), ([ ])),
        assert_equal(testOb->query_cooldown("test"), 0),

        // set a cooldown
        testOb->set_cooldown("test", 0.001, "timed"),
        assert_equal(testOb->query_cooldowns(), ([ "test": ([ "type": "timed", "value": 0.001 ]) ])),
        assert_equal(testOb->query_cooldown("test"), 0.001),
    }) :));

    call_out_walltime(function (function done) {
        expect("timed cooldowns expire", (: ({
            // cooldown expires after one heartbeat
            assert_equal(testOb->query_cooldowns(), ([ ])),
            assert_equal(testOb->query_cooldown("test"), 0),
        }) :));
        evaluate(done);
    }, 0.002, done);
}