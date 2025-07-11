inherit M_TEST;
inherit STD_LIVING;

/**
 * @var {"/std/npc/pet"} testOb
 */

void create () {
    ::create();
    set_id(({ "pet.test", }));
    set_name("pet.test");
}

void test_owner () {
    expect("owner is settable and queryable", (: ({
        // no owner set
        assert_equal(testOb->query_owner(), UNDEFINED),
        assert_equal(testOb->query_owner_name(), 0),

        testOb->set_owner(this_object()),
        // owner set
        assert_equal(testOb->query_owner(), this_object()),
        assert_equal(testOb->query_owner_name(), "pet.test"),
    }) :));
}

void test_following () {
    expect("following is settable and queryable", (: ({
        // not following yet
        assert_equal(testOb->query_following(), UNDEFINED),

        // following
        testOb->set_following(1),
        assert_equal(testOb->query_following(), 1),

        testOb->set_following(0),
        assert_equal(testOb->query_following(), 0),
    }) :));
}

void test_heart_beat () {
    object r1 = new(STD_ROOM);
    object r2 = new(STD_ROOM);
    object npc = new(STD_NPC);

    expect("test heart beat", (: ({
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(this_object()->handle_move($(r2)), 1),
        assert_equal($(npc)->handle_move($(r2)), 1),

        testOb->set_owner(this_object()),
        testOb->set_following(1),
        assert_equal(testOb->query_following(), 1),

        this_object()->add_hostile($(npc)),
        $(npc)->add_hostile(this_object()),
        assert_equal(this_object()->query_hostiles(), ({ $(npc) })),

        assert_equal(environment(testOb), $(r1)),
        testOb->heart_beat(),
        assert_equal(environment(testOb), $(r2)),
    }) :));

    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (npc) destruct(npc);
    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_parser_applies () {
    object r1 = new(STD_ROOM);
    object r2 = new(STD_ROOM);

    this_object()->handle_move(r1);
    expect("direct_lead_liv behaves", (: ({
        // different environment, fail
        assert_equal(testOb->direct_lead_liv(), 0),

        // same environment, success
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(testOb->direct_lead_liv(), 1),

    }) :));

    this_object()->handle_move(r2);
    expect("direct_ditch_liv behaves", (: ({
        // different environment, fail
        assert_equal(testOb->direct_ditch_liv(), 0),

        // same environment, success
        assert_equal(testOb->handle_move($(r2)), 1),
        assert_equal(testOb->direct_ditch_liv(), 1),
    }) :));

    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r1) destruct(r1);
    if (r2) destruct(r2);
}