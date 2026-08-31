inherit M_TEST;
inherit M_MOVE;
/**
 * @var {"/std/room"} testOb
 */

int is_item() { return 1; }  // need to fake being an item for room contents

void test_is_room() {
    expect("is_room returns true", (: ({
        assert_equal(testOb->is_room(), 1),
    }) :));
}

void test_clean_up() {
    object ob;

    ob = new(STD_ITEM);

    expect("clean_up removes objects", (: ({
        // no inventory
        assert_equal(all_inventory(testOb), ({})),
        // move item
        assert_equal($(ob)->handle_move(testOb), 1),
        // verify inventory
        assert_equal(sizeof(all_inventory(testOb)), 1),
        // clean item
        assert_equal(testOb->clean_up(), 1),
        // no inventory again
        assert_equal(testOb, 0),
        // verify object removed
        assert_equal(objectp($(ob)), 0),
    }) :));

    if (ob) destruct(ob);
}

void test_room_bracketing() {
    expect("room bracket color is queryable and settable", (: ({
        assert_equal(testOb->query_room_bracket_color(), ""),
        testOb->set_room_bracket_color("%^TEST%^"),
        assert_equal(testOb->query_room_bracket_color(), "%^TEST%^"),
    }) :));

    expect("room bracket is queryable and settable", (: ({
        assert_equal(testOb->query_room_brackets(), ({ "[", "]" })),
        testOb->set_room_brackets(({ "1", "2" })),
        assert_equal(testOb->query_room_brackets(), ({ "1", "2" })),
    }) :));
    expect("room bracket behaves with bad argument", (: ({
        assert_catch(
            (: testOb->set_room_brackets(({})) :),
            "*Bad argument 1 to room->set_room_brackets\n"
        ),
    }) :));

    expect("room map bracket is queryable", (: ({
        // default symbol
        assert_equal(
            testOb->query_room_map_bracket(),
            "%^TEST%^1%^RESET%^ %^TEST%^2%^RESET%^"
        ),

        // @TODO test with items/living

        // override symbol
        assert_equal(
            testOb->query_room_map_bracket("x"),
            "%^TEST%^1%^RESET%^x%^TEST%^2%^RESET%^"
        ),
        assert_equal(
            testOb->query_room_map_bracket("@"),
            "%^TEST%^1%^RESET%^@%^TEST%^2%^RESET%^"
        ),
    }) :));
}

void test_room_map_symbol() {
    object item1, item2;

    item1 = new(STD_ITEM);
    item2 = new(STD_ITEM);
    expect("room map symbol updates", (: ({
        // nothing in room
        assert_equal(testOb->query_room_map_symbol(), " "),

        // test objects
        assert_equal($(item1)->handle_move(testOb), 1),
        assert_equal(testOb->query_room_map_symbol(), "%^MAGENTA%^1%^RESET%^"),
        assert_equal($(item2)->handle_move(testOb), 1),
        assert_equal(testOb->query_room_map_symbol(), "%^MAGENTA%^2%^RESET%^"),

        // @TODO test passive NPC

        // @TODO test aggressive NPC
    }) :));

    if (item1) destruct(item1);
    if (item2) destruct(item2);
}

// this_character() resolves through previous_object()->query_character();
// defining it here lets query_room_map_symbol see a character during tests
nosave object __TestChar;
object query_character() {
    return __TestChar;
}

void test_room_map_symbol_living() {
    object npcAgg = new(STD_NPC), npcPass = new(STD_NPC);

    __TestChar = new(STD_NPC);
    __TestChar->set_stat("charisma", 0);
    npcAgg->set_aggressive(100);
    npcPass->set_aggressive(0);
    npcAgg->handle_move(testOb);
    npcPass->handle_move(testOb);

    expect("map symbol is orange with aggressive and passive livings", (: ({
        // charisma 0 < aggressive 100 -> aggressive; passive npc -> passive
        assert_regex(testOb->query_room_map_symbol(), "ORANGE"),
    }) :));

    npcPass->handle_move("/domain/Nowhere/room/void.c");
    expect("map symbol is red with only aggressive livings", (: ({
        assert_regex(testOb->query_room_map_symbol(), "I_RED"),
    }) :));

    npcAgg->handle_move("/domain/Nowhere/room/void.c");
    npcPass->handle_move(testOb);
    expect("map symbol is green with only passive livings", (: ({
        assert_regex(testOb->query_room_map_symbol(), "GREEN"),
    }) :));

    __TestChar = 0;
    if (npcAgg) destruct(npcAgg);
    if (npcPass) destruct(npcPass);
}

void test_room_exits_picture() {
    expect("room exits picture reflects exits and source filter", (: ({
        // no exits: blank
        assert_equal(testOb->query_room_exits_picture()["u"], " "),

        testOb->set_exits(([
            "up": "/domain/Nowhere/room/void.c",
            "down": "/domain/Nowhere/room/void.c",
        ])),
        assert_equal(testOb->query_room_exits_picture()["u"], "+"),
        assert_equal(testOb->query_room_exits_picture()["d"], "-"),

        // source filter that matches keeps the exit
        assert_equal(
            testOb->query_room_exits_picture(
                "/domain/Nowhere/room/void.c"
            )["u"],
            "+"
        ),
        // source filter that does not match blanks the exit
        assert_equal(
            testOb->query_room_exits_picture("/nonexistent.c")["u"],
            " "
        ),
    }) :));
}

void test_room_map() {
    expect("room map returns 0 when flagged no map", (: ({
        testOb->set_property("no map", 1),
        assert_equal(testOb->query_room_map(), 0),
        testOb->remove_property("no map"),
    }) :));

    expect("room map renders with and without a valid exit", (: ({
        // no exits: 9 rendered rows around the centered X
        assert_equal(sizeof(testOb->query_room_map()), 9),
        // a valid exit loads the neighbor room
        testOb->set_exits(([
            "north": "/domain/Nowhere/room/void.c",
        ])),
        assert_equal(sizeof(testOb->query_room_map()), 9),
    }) :));
}

void test_environment_damage() {
    object living = new(STD_LIVING);
    int i;

    living->handle_move(testOb);

    // no water property: nothing happens
    testOb->handle_environment_damage(living);

    // deep water: repeat to exercise the random damage case and the default
    testOb->set_property("water", 5);
    for (i = 0; i < 100; i++) {
        testOb->handle_environment_damage(living);
    }

    expect("environment damage runs in deep water", (: ({
        assert_equal(objectp(testOb), 1),
        assert_equal(objectp($(living)), 1),
    }) :));

    if (living) destruct(living);
}

nosave private int __HandleFnsItem = 0, __HandleFnsLiving = 0;
nosave object __Ob, __Living;

// Listen for receive/release events from the room
void handle_receive_living_in_env(object ob) {
    if (objectp(ob) && ob == __Living) {
        __HandleFnsLiving++;
    }
}
void handle_receive_item_in_env(object ob) {
    if (objectp(ob) && ob == __Ob) {
        __HandleFnsItem++;
    }
}
void handle_release_living_in_env(object ob) {
    if (objectp(ob) && ob == __Living) {
        __HandleFnsLiving++;
    }
}
void handle_release_item_in_env(object ob) {
    if (objectp(ob) && ob == __Ob) {
        __HandleFnsItem++;
    }
}
void test_handle_receive_and_release(function done) {
    __Ob = new(STD_ITEM);
    __Living = new(STD_LIVING);

    expect("handle_receive sets up room contents", (: ({
        // move this test, item, and living to the test room object
        assert_equal(this_object()->handle_move(testOb), 1),
        assert_equal(__Ob->handle_move(testOb), 1),
        assert_equal(__Living->handle_move(testOb), 1),
    }) :));
    call_out_walltime(function(function done) {
        expect("handle_receive informs room contents", (: ({
            // handle_receive should call local event handlers in this test
            assert_equal(testOb->handle_receive(__Living), 1),
            assert_equal(__HandleFnsLiving, 1),
            assert_equal(testOb->handle_receive(__Ob), 1),
            assert_equal(__HandleFnsItem, 1),
        }) :));

        expect("handle_release informs room contents", (: ({
            // handle_release should call local event handlers in this test
            assert_equal(testOb->handle_release(__Living), 1),
            assert_equal(__HandleFnsLiving, 2),
            assert_equal(testOb->handle_release(__Ob), 1),
            assert_equal(__HandleFnsItem, 2),
            // prepare to destruct
            assert_equal(
                this_object()->handle_move("/domain/Nowhere/room/void.c"),
                1
            ),
        }) :));

        if (__Ob) destruct(__Ob);
        if (__Living) destruct(__Living);

        evaluate(done);
    }, 0, done);
}
