inherit M_TEST;
inherit STD_OBJECT;

/**
 * @var {"/std/item/bodypart"} testOb
 */

private mixed *calloutInfo;
void test_received () {
    expect("handle_received sets expire timer", (: ({
        assert_regex(file_name(testOb), "/std/item/bodypart.coverage#[0-9]+"),

        // start the expire call_out
        testOb->handle_received(this_object()),

        // grab callout tied to the test corpse
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 1),
        assert_equal(sizeof(calloutInfo[0]), 3),
        assert_equal(calloutInfo[0][0], testOb),
    }) :));
}

void test_bodypart () {
    object room = new(STD_ROOM);

    handle_move(room);
    expect("bodypart is setup to match", (: ({
        set_name("someone"),
        assert_equal(query_name(), "someone"),

        testOb->setup_bodypart(this_object(), "some limb"),
        // verify we set bodypart information
        assert_equal(testOb->query_name(), "piece of some limb"),
        assert_equal(testOb->query_short(), "piece of Someone's some limb"),
        assert_equal(testOb->query_id(), ({ "bodypart", "body part", "limb", "pieceofsomelimb", "piece of some limb", "some limb", })),
    }) :));
    handle_move("/domain/Nowhere/room/void.c");
    if (room) destruct(room);
}