inherit M_TEST;

/**
 * @var {"/std/living/status"} testOb
 */

#define STATUS_MOCK "/std/living/status.c" & "/std/living/status.mock.c"

void test_busy () {
    expect("busy should be settable and queryable", (: ({
        assert_equal(testOb->query_busy(), 0),

        testOb->set_busy(2),
        assert_equal(testOb->query_busy(), 2),
        testOb->heart_beat(),
        assert_equal(testOb->query_busy(), 1),
        testOb->heart_beat(),
        assert_equal(testOb->query_busy(), 0),
        testOb->heart_beat(),
        assert_equal(testOb->query_busy(), 0), // still zero
    }) :));
}

void test_disable () {
    expect("disable should be settable and queryable", (: ({
        assert_equal(testOb->query_disable(), 0),

        testOb->set_disable(2),
        assert_equal(testOb->query_disable(), 2),
        testOb->heart_beat(),
        assert_equal(testOb->query_disable(), 1),
        testOb->heart_beat(),
        assert_equal(testOb->query_disable(), 0),
        testOb->heart_beat(),
        assert_equal(testOb->query_disable(), 0), // still zero
    }) :));
}

void test_immobile () {
    object r1, r2;

    expect("immobile should be settable and queryable", (: ({
        assert_equal(testOb->query_immobile(), 0),

        testOb->set_immobile(2),
        assert_equal(testOb->query_immobile(), 2),
        testOb->heart_beat(),
        assert_equal(testOb->query_immobile(), 1),
        testOb->heart_beat(),
        assert_equal(testOb->query_immobile(), 0),
        testOb->heart_beat(),
        assert_equal(testOb->query_immobile(), 0), // still zero
    }) :));

    destruct(testOb);
    testOb = new(STD_LIVING);
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    expect("immobile should prevent handle_command", (: ({
        assert_equal(/** @type {M_MOVE} */ (testOb)->handle_move($(r1)), 1),
        assert_equal(testOb->query_immobile(), 0),

        testOb->set_immobile(2),
        assert_equal(testOb->query_immobile(), 2),
        assert_equal(/** @type {STD_LIVING} */ (testOb)->handle_command("go east"), 1), // command to move to r2
        assert_equal(environment(testOb), $(r1)), // still r1
    }) :));

    destruct(testOb);
    destruct(r1);
    destruct(r2);
}

void test_posture () {
    object mockBody;

    expect("posture should be settable and queryable", (: ({
        assert_equal(testOb->query_posture(), "standing"),

        testOb->set_posture("sitting"),
        assert_equal(testOb->query_posture(), "sitting"),
        testOb->set_posture("laying"),
        assert_equal(testOb->query_posture(), "laying"),
        testOb->set_posture("standing"),
        assert_equal(testOb->query_posture(), "standing"),

        assert_catch((: testOb->set_posture("unknown") :), "*Bad argument 1 to status->set_posture\n"),
    }) :));

    mockBody = new("/std/living/status.mock.c");
    mockBody->start_shadow(testOb);
    expect("postures should adjust vitals on heart_beat", (: ({
        // sitting
        testOb->set_posture("sitting"),
        assert_equal(testOb->query_posture(), "sitting"),
        testOb->heart_beat(),
        assert_equal(/** @type {STATUS_MOCK} */ (testOb)->query_mock_heals(), ({ 1 })),
        /** @type {STATUS_MOCK} */ (testOb)->reset_mock_heals(),

        // laying
        testOb->set_posture("laying"),
        assert_equal(testOb->query_posture(), "laying"),
        testOb->heart_beat(),
        assert_equal(/** @type {STATUS_MOCK} */ (testOb)->query_mock_heals(), ({ 2 })),
        /** @type {STATUS_MOCK} */ (testOb)->reset_mock_heals(),

        // meditating
        testOb->set_posture("meditating"),
        assert_equal(testOb->query_posture(), "meditating"),
        testOb->heart_beat(),
        assert_equal(/** @type {STATUS_MOCK} */ (testOb)->query_mock_heals(), ({ 2 })),
        /** @type {STATUS_MOCK} */ (testOb)->reset_mock_heals(),
        // verify we gain mp
        assert_equal(/** @type {STATUS_MOCK} */ (testOb)->query_mock_add_vitals()["mp"] > 0, 1),
        /** @type {STATUS_MOCK} */ (testOb)->query_mock_add_vitals(),

        // flying
        testOb->set_posture("flying"),
        assert_equal(testOb->query_posture(), "flying"),
        testOb->heart_beat(),
        // verify we lost sp
        assert_equal(/** @type {STATUS_MOCK} */ (testOb)->query_mock_add_vitals()["sp"] < 0, 1),
        /** @type {STATUS_MOCK} */ (testOb)->query_mock_add_vitals(),
    }) :));

    mockBody->stop_shadow();
    if (mockBody) destruct(mockBody);
}