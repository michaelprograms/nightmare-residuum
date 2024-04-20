inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

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

    expect("immobile should prevent do_command", (: ({
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(testOb->query_immobile(), 0),

        testOb->set_immobile(2),
        assert_equal(testOb->query_immobile(), 2),
        assert_equal(testOb->do_command("go east"), 1), // command to move to r2
        assert_equal(environment(testOb), $(r1)), // still r1
    }) :));

    destruct(testOb);
    destruct(r1);
    destruct(r2);
}

void test_posture () {
    expect("posture should be settable and queryable", (: ({
        assert_equal(testOb->query_posture(), "standing"),

        testOb->set_posture("sitting"),
        assert_equal(testOb->query_posture(), "sitting"),
        testOb->set_posture("laying"),
        assert_equal(testOb->query_posture(), "laying"),
        testOb->set_posture("standing"),
        assert_equal(testOb->query_posture(), "standing"),
    }) :));

    // @TODO
    // expect("postures should heal on heartbeat", (: ({
        // resting

        // sleeping

    // }) :));
}