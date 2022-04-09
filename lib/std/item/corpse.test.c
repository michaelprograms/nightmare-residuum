inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/item/corpse.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_expire () {
    expect_function("handle_expire", testOb);

    expect("expire to remove corpse", (: ({
        assert(undefinedp(testOb), "==", 0),
        testOb->handle_expire(),
        assert(undefinedp(testOb), "==", 1),
    }) :));
}

private string testObFile;
private mixed *calloutInfo;
void test_received () {
    function_exists("handle_received", testOb);

    expect("handle_received sets expire timer", (: ({
        assert(testObFile = file_name(testOb), "regex", "/std/item/corpse#\d"),

        // call handle_received to start the expire call_out
        testOb->handle_received(0),

        // grab callout tied to the test corpse
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert(sizeof(calloutInfo), "==", 1),
        assert(sizeof(calloutInfo[0]), "==", 3),
        assert(calloutInfo[0][0], "==", testOb),
    }) :));
}