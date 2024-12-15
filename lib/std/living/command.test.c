inherit M_TEST;

/**
 * @var {"/std/living/command"} testOb
 */

void test_unknown_response () {
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