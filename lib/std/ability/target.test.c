inherit M_TEST;

/**
 * @var {"/std/ability/target"} testOb
 */

void test_targets () {
    expect("handles setting and querying targets", (: ({
        // default
        assert_equal(testOb->query_targets(), 1),

        testOb->set_targets(2),
        assert_equal(testOb->query_targets(), 2),

        testOb->set_targets(10),
        assert_equal(testOb->query_targets(), 10),

        assert_catch((: testOb->set_targets(0) :), "*Bad argument 1 to ability->set_targets\n"),
    }) :));
}
