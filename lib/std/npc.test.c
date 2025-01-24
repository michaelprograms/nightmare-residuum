inherit M_TEST;
/**
 * @var {"/std/npc"} testOb
 */

void test_npc () {
    expect("is_npc behaves", (: ({
        assert_equal(livingp(testOb), 1),
        assert_equal(npcp(testOb), 1),
        assert_equal(characterp(testOb), 0),
    }) :));
}

void test_set_level () {
    expect("set_level adjusts stats", (: ({
        assert_equal(testOb->query_level(), 1),
        assert_equal(testOb->query_stat("strength"), 0),
        assert_equal(testOb->query_stat("endurance"), 0),

        testOb->set_level(10),
        assert_equal(testOb->query_level(), 10),
        assert_equal(testOb->query_stat("strength") > 0, 1),
        assert_equal(testOb->query_stat("endurance") > 0, 1),
    }) :));
}

nosave object __Room, __Character;
void test_aggressive (function done) {
    expect("aggressive is settable and queryable", (: ({
        assert_equal(testOb->query_aggressive(), 0),

        testOb->set_aggressive(10),
        assert_equal(testOb->query_aggressive(), 10),

        testOb->set_aggressive(123),
        assert_equal(testOb->query_aggressive(), 123),
    }) :));

    __Room = new(STD_ROOM);
    __Character = new(STD_CHARACTER);
    __Character->set_level(1);
    __Character->set_stat("charisma", 1);

    expect("aggressive pre-requisites are met", (: ({
        assert_equal(__Character->query_level(), 1),
        assert_equal(__Character->query_stat("charisma"), 1),

        assert_equal(testOb->query_hostile(__Character), 0),
        assert_equal(__Character->query_hostile(testOb), 0),

        assert_equal(testOb->handle_move(__Room), 1),
        assert_equal(__Character->handle_move(__Room), 1),
    }) :));

    call_out_walltime(function (function done) {
        expect("aggressive initiates combat", (: ({
            assert_equal(testOb->query_hostile(__Character), 1),
            assert_equal(__Character->query_hostile(testOb), 1),
        }) :));

        if (__Room) destruct(__Room);
        if (__Character) destruct(__Character);

        evaluate(done);
    }, 0, done);
}

nosave private object r1, r2;

void test_wander () {
    expect("wander is queryable and settable", (: ({
        assert_equal(testOb->query_wander(), 0),

        testOb->set_wander(1),
        assert_equal(testOb->query_wander(), 1),
        assert_equal(testOb->query_next_wander(), 0),

        testOb->set_wander(10),
        assert_equal(testOb->query_wander(), 10),
        assert_equal(testOb->query_next_wander(), 0),

        testOb->set_wander(50),
        assert_equal(testOb->query_wander(), 50),
        assert_equal(testOb->query_next_wander(), 0),

        testOb->set_wander(0),
        assert_equal(testOb->query_wander(), 0),
        assert_equal(testOb->query_next_wander(), 0),
    }) :));

    expect("wander sets up next_wander", (: ({
        testOb->set_next_wander(10),
        assert_equal(testOb->query_next_wander(), 10),

        testOb->set_next_wander(50),
        assert_equal(testOb->query_next_wander(), 50),
    }) :));

    // setup rooms for wandering test
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    expect("wander moves to neighboring room", (: ({
        assert_equal(testOb->handle_move(r1), 1),

        // wander not set up, won't move
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r1),
        assert_equal(testOb->query_wanders(), 0),

        // setup wander value of 1 (moves on 2nd attempt)
        testOb->set_wander(1),
        assert_equal(testOb->query_next_wander(), 0),
        // won't move on first attempt
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r1),
        assert_equal(testOb->query_next_wander(), 1),
        assert_equal(testOb->query_wanders(), 0),
        // moves
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r2),
        assert_equal(testOb->query_next_wander(), 0),
        assert_equal(testOb->query_wanders(), 1),

        // setup wander value of 3 (moves on 4th attempt)
        testOb->set_wander(3),
        assert_equal(testOb->query_next_wander(), 0),
        // won't move on first three attempts
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r2),
        assert_equal(testOb->query_next_wander(), 1),
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r2),
        assert_equal(testOb->query_next_wander(), 2),
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r2),
        assert_equal(testOb->query_next_wander(), 3),
        // moves
        testOb->attempt_wander(),
        assert_equal(environment(testOb), r1),
        assert_equal(testOb->query_next_wander(), 0),
        assert_equal(testOb->query_wanders(), 2),

        // skipping checks and moves
        testOb->handle_wander(),
        assert_equal(environment(testOb), r2),
        // values don't increment
        assert_equal(testOb->query_next_wander(), 0),
        assert_equal(testOb->query_wanders(), 2),
    }) :));

    destruct(r1);
    destruct(r2);
}

void test_abilities () {
    object mockNPC = new("/std/npc.mock.c");

    expect("ability list is queryable and settable", (: ({
        assert_equal(testOb->query_ability_list(), ({ })),

        testOb->set_ability_list(({ "test1", "test2", })),
        assert_equal(testOb->query_ability_list(), ({ "test1", "test2", })),

        testOb->set_ability_list(({ "testA", "testB", })),
        assert_equal(testOb->query_ability_list(), ({ "testA", "testB", })),
    }) :));

    expect("ability chance is queryable and settable", (: ({
        assert_equal(testOb->query_ability_chance(), 0),

        testOb->set_ability_chance(123),
        assert_equal(testOb->query_ability_chance(), 100),

        testOb->set_ability_chance(25),
        assert_equal(testOb->query_ability_chance(), 25),

        testOb->set_ability_chance(-123),
        assert_equal(testOb->query_ability_chance(), 0),
    }) :));

    expect("ability should be commanded", (: ({
        assert_equal($(mockNPC)->start_shadow(testOb), 1),

        // no ability list
        testOb->set_ability_list(({ })),
        assert_equal(testOb->query_ability_list(), ({ })),
        testOb->set_ability_chance(100),
        assert_equal(testOb->query_ability_chance(), 100),
        testOb->handle_ability_attack(),
        // nothing commanded
        assert_equal(testOb->query_received_commands(), ({ })),

        // ability list
        testOb->set_ability_list(({ "test ability" })),
        testOb->handle_ability_attack(),
        // ability was commanded
        assert_equal(testOb->query_received_commands(), ({ "test ability" })),

        assert_equal($(mockNPC)->stop_shadow(), 1),
    }) :));

    if (mockNPC) destruct(mockNPC);
}

void test_say_response () {
    object mockNPC = new("/std/npc.mock.c");

    expect("say responses should be handled", (: ({
        assert_equal($(mockNPC)->start_shadow(testOb), 1),

        assert_equal(testOb->query_say_response(), ([ ])),

        testOb->set_say_response("match", "response"),
        assert_equal(testOb->query_say_response(), ([ "match": "response" ])),
        assert_equal(testOb->query_say_response_matches(), ({ "match" })),
        // nothing commanded
        assert_equal(testOb->query_received_commands(), ({ })),

        testOb->handle_say_response("something unrelated"),
        // nothing commanded still
        assert_equal(testOb->query_received_commands(), ({ })),

        testOb->handle_say_response("match"),
        // say response was commanded
        assert_equal(testOb->query_received_commands(), ({ "say response" })),

        testOb->receive_message("say", "You ask: match?"),
        testOb->receive_message("say", "You exclaim: match!"),
        testOb->receive_message("say", "You say: match"),
        testOb->receive_message("say", "You synthesize: match"),
        assert_equal(testOb->query_received_commands(), ({ "say response" })),
        // matches off another living
        testOb->receive_message("say", "Someone says: match"),
        assert_equal(testOb->query_received_commands(), ({ "say response", "say response" })),
        testOb->receive_message("say", "Someone says: match"),
        assert_equal(testOb->query_received_commands(), ({ "say response", "say response", "say response" })),

        assert_equal($(mockNPC)->stop_shadow(), 1),
    }) :));

    if (mockNPC) destruct(mockNPC);
}