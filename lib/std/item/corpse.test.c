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
        assert(testObFile = file_name(testOb), "regex", "/std/item/corpse#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(0),

        // grab callout tied to the test corpse
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert(sizeof(calloutInfo), "==", 1),
        assert(sizeof(calloutInfo[0]), "==", 3),
        assert(calloutInfo[0][0], "==", testOb),
    }) :));
}

void test_corpse () {
    expect_function("is_corpse", testOb);

    expect("is_corpse behaves", (: ({
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_corpse(), "==", 1),
        assert(testOb->is_character(), "==", UNDEFINED),
        assert(testOb->is_monster(), "==", UNDEFINED),
        assert(testOb->is_npc(), "==", UNDEFINED),
    }) :));
}