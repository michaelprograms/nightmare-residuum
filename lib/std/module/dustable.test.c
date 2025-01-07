inherit M_TEST;

/**
 * @var {"/std/module/dustable"} testOb
 */

void test_expire () {
    object mockItem = new("/std/module/dustable.mock.c");
    expect("expire to remove dustable", (: ({
        assert_equal($(mockItem)->start_shadow(testOb), 1),
        // @lpc-ignore
        assert_equal(testOb->query_destructed(), 0), // mock method
        testOb->handle_expire(),
        // @lpc-ignore
        assert_equal(testOb->query_destructed(), 1), // mock method
        assert_equal($(mockItem)->stop_shadow(), 1),
    }) :));
    if (mockItem) destruct(mockItem);
}

private mixed *calloutInfo;
void test_received () {
    function_exists("handle_received", testOb);

    expect("handle_received sets expire timer", (: ({
        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test dustable
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 1),
        assert_equal(sizeof(calloutInfo[0]), 3),
        assert_equal(calloutInfo[0][0], testOb),
    }) :));
}