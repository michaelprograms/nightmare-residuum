inherit M_TEST;

/**
 * @var {"/std/living/command"} testOb
 */

void test_unknown_response() {
    expect("unknown commands are responded to", (: ({
        // no verb called
        assert_equal(testOb->query_unknown_command(0), 0),
        // verb called
        assert_equal(testOb->query_unknown_command(1), 1),

        assert_equal(testOb->query_unknown_command(-1), 1),
        assert_equal(testOb->query_unknown_command(-2), 1),
        assert_equal(testOb->query_unknown_command(-123), 1),
    }) :));
}

void test_normalize_command() {
    expect("enter/out are rewritten to go, others untouched", (: ({
        assert_equal(testOb->normalize_command("enter door"), "go enter door"),
        assert_equal(testOb->normalize_command("out"), "go out"),
        assert_equal(testOb->normalize_command("outpost"), "go outpost"),
        assert_equal(testOb->normalize_command("north"), "north"),
        assert_equal(testOb->normalize_command("go west"), "go west"),
    }) :));
}

void test_is_reportable_result() {
    expect("benign go-failures are suppressed, real errors reported", (: ({
        assert_equal(testOb->is_reportable_result("You can't go that way."), 0),
        assert_equal(testOb->is_reportable_result("There is no exit."), 0),
        assert_equal(testOb->is_reportable_result("Some other error."), 1),
        assert_equal(testOb->is_reportable_result(0), 0),
        assert_equal(testOb->is_reportable_result(1), 0),
    }) :));
}

void test_handle_command_no_env() {
    expect("handle_command returns 0 with no environment or command", (: ({
        assert_equal(testOb->handle_command("north"), 0),
        assert_equal(testOb->handle_command(0), 0),
    }) :));
}

void test_handle_command() {
    /** @type {"/std/living/command.mock"} */
    object mock = new("/std/living/command.mock.c");
    /** @type {STD_ROOM} */
    object room = new(STD_ROOM);

    room->set_exits(([ "north": "/domain/Nowhere/room/void.c" ]));
    mock->mock_move(room);

    expect("handle_command routes commands, verbs, and exits", (: ({
        // command
        assert_equal($(mock)->handle_command("idle"), 1),

        // verb that fails returns its message
        assert_equal($(mock)->handle_command("go"), 1),

        // verb that succeeds
        assert_equal($(mock)->handle_command("say hi"), 1),

        // unknown
        assert_equal($(mock)->handle_command("xyzzy"), 0),

        // exit shortcut
        assert_equal($(mock)->handle_command("north"), 1),

        // exit shortcut that fails reports the go failure
        $(mock)->set_posture("laying"),
        assert_equal($(mock)->handle_command("north"), 1),

        // immobile
        $(mock)->set_immobile(1),
        assert_equal($(mock)->handle_command("north"), 1),
        assert_equal(
            $(mock)->query_received_messages()[<1],
            ({ "action", "You are unable to take any actions right now." })
        ),
    }) :));

    destruct(mock);
    destruct(room);
}
