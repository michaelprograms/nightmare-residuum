inherit M_TEST;
inherit M_MOVE;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/room.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

int is_item () { return 1; } // need to fake being an item for room contents

void test_is_room () {
    expect_function("is_room", testOb);

    expect("is_room returns true", (: ({
        assert(testOb->is_room(), "==", 1),
    }) :));
}

void test_clean_up () {
    object ob;

    expect_function("clean_up", testOb);

    ob = new(STD_ITEM);

    expect("clean_up removes objects", (: ({
        // no inventory
        assert(all_inventory(testOb), "==", ({ })),
        // move item
        assert($(ob)->handle_move(testOb), "==", 1),
        // verify inventory
        assert(sizeof(all_inventory(testOb)), "==", 1),
        // clean item
        assert(testOb->clean_up(), "==", 1),
        // no inventory again
        assert(testOb, "==", 0),
        // verify object removed
        assert(objectp($(ob)), "==", 0),
    }) :));

    if (ob) destruct(ob);
}

void test_room_bracketing () {
    expect_function("set_room_bracket_color", testOb);
    expect_function("query_room_bracket_color", testOb);
    expect_function("set_room_brackets", testOb);
    expect_function("query_room_brackets", testOb);

    expect("room bracket color is queryable and settable", (: ({
        assert(testOb->query_room_bracket_color(), "==", ""),
        testOb->set_room_bracket_color("%^TEST%^"),
        assert(testOb->query_room_bracket_color(), "==", "%^TEST%^"),
    }) :));

    expect("room bracket is queryable and settable", (: ({
        assert(testOb->query_room_brackets(), "==", ({ "[", "]" })),
        testOb->set_room_brackets(({ "1", "2" })),
        assert(testOb->query_room_brackets(), "==", ({ "1", "2" })),
    }) :));
}

nosave private int __HandleFnsItem = 0, __HandleFnsLiving = 0;
nosave object __Ob, __Living;

// Listen for receive/release events from the room
void handle_receive_living_in_env (object ob) {
    if (objectp(ob) && ob == __Living) {
        __HandleFnsLiving ++;
    }
}
void handle_receive_item_in_env (object ob) {
    if (objectp(ob) && ob == __Ob) {
        __HandleFnsItem ++;
    }
}
void handle_release_living_in_env (object ob) {
    if (objectp(ob) && ob == __Living) {
        __HandleFnsLiving ++;
    }
}
void handle_release_item_in_env (object ob) {
    if (objectp(ob) && ob == __Ob) {
        __HandleFnsItem ++;
    }
}
void test_handle_receive_and_release (function done) {
    expect_function("handle_receive", testOb);
    expect_function("handle_release", testOb);

    __Ob = new(STD_ITEM);
    __Living = new(STD_LIVING);

    expect("handle_receive sets up room contents", (: ({
        // move this test, item, and living to the test room object
        assert(this_object()->handle_move(testOb), "==", 1),
        assert(__Ob->handle_move(testOb), "==", 1),
        assert(__Living->handle_move(testOb), "==", 1),
    }) :));
    call_out_walltime(function (function done) {
        expect("handle_receive informs room contents", (: ({
            // handle_receive should call local event handlers in this test
            assert(testOb->handle_receive(__Living), "==", 1),
            assert(__HandleFnsLiving, "==", 1),
            assert(testOb->handle_receive(__Ob), "==", 1),
            assert(__HandleFnsItem, "==", 1),
        }) :));

        expect("handle_release informs room contents", (: ({
            // handle_release should call local event handlers in this test
            assert(testOb->handle_release(__Living), "==", 1),
            assert(__HandleFnsLiving, "==", 2),
            assert(testOb->handle_release(__Ob), "==", 1),
            assert(__HandleFnsItem, "==", 2),
            // prepare to destruct
            assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        }) :));

        if (__Ob) destruct(__Ob);
        if (__Living) destruct(__Living);

        evaluate(done);
    }, 0, done);
}