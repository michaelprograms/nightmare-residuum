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
