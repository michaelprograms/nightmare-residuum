inherit M_TEST;

void test_shadow () {
    expect("shadow handled starting and stopping", (: ({
        assert_equal(testOb->stop_shadow(), UNDEFINED),
        assert_equal(testOb->query_shadow(), UNDEFINED),
        assert_equal(testOb->start_shadow(this_object()), 1),
        assert_equal(testOb->query_shadow(), this_object()),
        assert_equal(testOb->stop_shadow(), 1),
    }) :));
}