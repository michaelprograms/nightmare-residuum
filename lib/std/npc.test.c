inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/npc.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_npc () {
    expect_function("is_npc", testOb);

    expect("is_npc behaves", (: ({
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_npc(), "==", 1),
        assert(testOb->is_character(), "==", UNDEFINED),
    }) :));
}

void test_set_level () {
    expect_function("set_level", testOb);

    expect("set_level adjusts stats", (: ({
        assert(testOb->query_level(), "==", 1),
        assert(testOb->query_stat("strength"), "==", 0),
        assert(testOb->query_stat("endurance"), "==", 0),

        testOb->set_level(10),
        assert(testOb->query_level(), "==", 10),
        assert(testOb->query_stat("strength"), ">", 0),
        assert(testOb->query_stat("endurance"), ">", 0),
    }) :));
}

nosave object __Room, __Character;
void test_aggressive (function done) {
    expect_function("set_aggressive", testOb);
    expect_function("query_aggressive", testOb);
    expect_function("handle_receive_living_in_env", testOb);

    expect("aggressive is settable and queryable", (: ({
        assert(testOb->query_aggressive(), "==", 0),

        testOb->set_aggressive(10),
        assert(testOb->query_aggressive(), "==", 10),

        testOb->set_aggressive(123),
        assert(testOb->query_aggressive(), "==", 123),
    }) :));

    __Room = new(STD_ROOM);
    __Character = new(STD_CHARACTER);
    __Character->set_level(1);
    __Character->set_stat("charisma", 1);

    expect("aggressive pre-requisites are met", (: ({
        assert(__Character->query_level(), "==", 1),
        assert(__Character->query_stat("charisma"), "==", 1),

        assert(testOb->query_hostile(__Character), "==", 0),
        assert(__Character->query_hostile(testOb), "==", 0),

        assert(testOb->handle_move(__Room), "==", 1),
        assert(__Character->handle_move(__Room),  "==", 1),
    }) :));

    call_out_walltime(function (function done) {
        expect("aggressive initiates combat", (: ({
            assert(testOb->query_hostile(__Character), "==", 1),
            assert(__Character->query_hostile(testOb), "==", 1),
        }) :));

        if (__Room) destruct(__Room);
        if (__Character) destruct(__Character);

        evaluate(done);
    }, 0, done);
}

nosave private object r1, r2;

void test_wander () {
    expect_function("set_wander", testOb);
    expect_function("query_wander", testOb);
    expect_function("query_wanders", testOb);
    expect_function("set_next_wander", testOb);
    expect_function("query_next_wander", testOb);
    expect_function("attempt_wander", testOb);
    expect_function("handle_wander", testOb);

    expect("wander is queryable and settable", (: ({
        assert(testOb->query_wander(), "==", 0),

        testOb->set_wander(1),
        assert(testOb->query_wander(), "==", 1),
        assert(testOb->query_next_wander(), "==", 0),

        testOb->set_wander(10),
        assert(testOb->query_wander(), "==", 10),
        assert(testOb->query_next_wander(), "==", 0),

        testOb->set_wander(50),
        assert(testOb->query_wander(), "==", 50),
        assert(testOb->query_next_wander(), "==", 0),

        testOb->set_wander(0),
        assert(testOb->query_wander(), "==", 0),
        assert(testOb->query_next_wander(), "==", 0),
    }) :));

    expect("wander sets up next_wander", (: ({
        testOb->set_next_wander(10),
        assert(testOb->query_next_wander(), "==", 10),

        testOb->set_next_wander(50),
        assert(testOb->query_next_wander(), "==", 50),
    }) :));

    // setup rooms for wandering test
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    expect("wander moves to neighboring room", (: ({
        assert(testOb->handle_move(r1), "==", 1),

        // wander not set up, won't move
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r1),
        assert(testOb->query_wanders(), "==", 0),

        // setup wander value of 1 (moves on 2nd attempt)
        testOb->set_wander(1),
        assert(testOb->query_next_wander(), "==", 0),
        // won't move on first attempt
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r1),
        assert(testOb->query_next_wander(), "==", 1),
        assert(testOb->query_wanders(), "==", 0),
        // moves
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r2),
        assert(testOb->query_next_wander(), "==", 0),
        assert(testOb->query_wanders(), "==", 1),

        // setup wander value of 3 (moves on 4th attempt)
        testOb->set_wander(3),
        assert(testOb->query_next_wander(), "==", 0),
        // won't move on first three attempts
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r2),
        assert(testOb->query_next_wander(), "==", 1),
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r2),
        assert(testOb->query_next_wander(), "==", 2),
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r2),
        assert(testOb->query_next_wander(), "==", 3),
        // moves
        testOb->attempt_wander(),
        assert(environment(testOb), "==", r1),
        assert(testOb->query_next_wander(), "==", 0),
        assert(testOb->query_wanders(), "==", 2),

        // skipping checks and moves
        testOb->handle_wander(),
        assert(environment(testOb), "==", r2),
        // values don't increment
        assert(testOb->query_next_wander(), "==", 0),
        assert(testOb->query_wanders(), "==", 2),
    }) :));

    destruct(r1);
    destruct(r2);
}