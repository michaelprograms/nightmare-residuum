inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/item/bodypart.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

private string testObFile;
private mixed *calloutInfo;
void test_received () {
    function_exists("handle_received", testOb);

    expect("handle_received sets expire timer", (: ({
        assert(testObFile = file_name(testOb), "regex", "/std/item/bodypart#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test corpse
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert(sizeof(calloutInfo), "==", 1),
        assert(sizeof(calloutInfo[0]), "==", 3),
        assert(calloutInfo[0][0], "==", testOb),
    }) :));
}

void test_bodypart () {
    expect_function("setup_bodypart", testOb);

    expect("bodypart is setup to match", (: ({
        testOb->setup_bodypart("Someone", "some limb"),
        // verify we set bodypart information
        assert(testOb->query_name(), "==", "piece of some limb"),
        assert(testOb->query_short(), "==", "a piece of Someone's some limb"),
        assert(testOb->query_id(), "==", ({ "bodypart", "body part", "limb", "pieceofsomelimb", "piece of some limb", "some limb", })),
    }) :));
}