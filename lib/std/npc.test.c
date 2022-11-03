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
        assert(testOb->is_monster(), "==", UNDEFINED),
        assert(testOb->is_character(), "==", UNDEFINED),
    }) :));
}

void test_set_level () {
    expect_function("set_level", testOb);

    expect("set_level adjusts stats and skills", (: ({
        assert(testOb->query_level(), "==", 1),
        assert(testOb->query_stat("strength"), "==", 0),
        assert(testOb->query_stat("endurance"), "==", 0),
        assert(testOb->query_skill("melee attack"), "==", 0),
        assert(testOb->query_skill("melee defense"), "==", 0),

        testOb->set_level(10),
        assert(testOb->query_level(), "==", 10),
        assert(testOb->query_stat("strength"), ">", 0),
        assert(testOb->query_stat("endurance"), ">", 0),
        assert(testOb->query_skill("melee attack"), ">", 0),
        assert(testOb->query_skill("melee defense"), ">", 0),
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