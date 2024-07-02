inherit M_TEST;

void test_expire () {
    object mockItem = new("/std/mock/item.c");

    expect("expire to remove dustable", (: ({
        assert_equal($(mockItem)->start_shadow(testOb), 1),
        assert_equal(testOb->query_destructed(), 0), // /std/mock/item method
        testOb->handle_expire(),
        assert_equal(testOb->query_destructed(), 1), // /std/mock/item method
        assert_equal($(mockItem)->stop_shadow(), 1),
    }) :));
    if (mockItem) destruct(mockItem);
}

private mixed *calloutInfo;
void test_received () {
    function_exists("handle_received", testOb);

    expect("handle_received sets expire timer", (: ({
        assert_regex(file_name(testOb), "/std/module/dustable\\.coverage#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test dustable
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 1),
        assert_equal(sizeof(calloutInfo[0]), 3),
        assert_equal(calloutInfo[0][0], testOb),
    }) :));
}