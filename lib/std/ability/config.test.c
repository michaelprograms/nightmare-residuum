inherit M_TEST;

/**
 * @var {"/std/ability/config"} testOb
 */

void test_type () {
    expect("handles setting and querying type", (: ({
        assert_equal(testOb->query_type(), UNDEFINED),

        testOb->set_type("attack"),
        assert_equal(testOb->query_type(), "attack"),

        testOb->set_type("heal"),
        assert_equal(testOb->query_type(), "heal"),

        testOb->set_type("utility"),
        assert_equal(testOb->query_type(), "utility"),

        assert_catch((: testOb->set_type("invalid") :), "*Bad argument 1 to ability/config->set_type\n"),
    }) :));
}
