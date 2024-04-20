inherit M_TEST;
inherit M_MOVE;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

int is_item () { return 1; } // need to fake being an item for room contents

void test_is_room () {
    expect("is_room returns true", (: ({
        assert_equal(testOb->is_room(), 1),
    }) :));
}

void test_clean_up () {
    object ob;

    ob = new(STD_ITEM);

    expect("clean_up removes objects", (: ({
        // no inventory
        assert_equal(all_inventory(testOb), ({ })),
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

void test_room_bracketing () {
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

    expect("room map bracket is queryable", (: ({
        // default symbol
        assert_equal(testOb->query_room_map_bracket(), "%^TEST%^1%^RESET%^ %^TEST%^2%^RESET%^"),

        // @TODO test with items/living

        // override symbol
        assert_equal(testOb->query_room_map_bracket("x"), "%^TEST%^1%^RESET%^x%^TEST%^2%^RESET%^"),
        assert_equal(testOb->query_room_map_bracket("@"), "%^TEST%^1%^RESET%^@%^TEST%^2%^RESET%^"),
    }) :));
}

void test_room_map_symbol () {
    object item1, item2;

    item1 = new(STD_ITEM);
    item2 = new(STD_ITEM);
    expect("room map symbol updates", (: ({
        // nothing in room
        assert_equal(testOb->query_room_map_symbol(), " "),

        // test objects
        assert_equal($(item1)->handle_move(testOb), 1),
        assert_equal(testOb->query_room_map_symbol(), "%^MAGENTA%^BOLD%^1%^RESET%^"),
        assert_equal($(item2)->handle_move(testOb), 1),
        assert_equal(testOb->query_room_map_symbol(), "%^MAGENTA%^BOLD%^2%^RESET%^"),

        // @TODO test passive NPC

        // @TODO test aggressive NPC
    }) :));

    if (item1) destruct(item1);
    if (item2) destruct(item2);
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
    __Ob = new(STD_ITEM);
    __Living = new(STD_LIVING);

    expect("handle_receive sets up room contents", (: ({
        // move this test, item, and living to the test room object
        assert_equal(this_object()->handle_move(testOb), 1),
        assert_equal(__Ob->handle_move(testOb), 1),
        assert_equal(__Living->handle_move(testOb), 1),
    }) :));
    call_out_walltime(function (function done) {
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
            assert_equal(this_object()->handle_move("/domain/Nowhere/room/void.c"), 1),
        }) :));

        if (__Ob) destruct(__Ob);
        if (__Living) destruct(__Living);

        evaluate(done);
    }, 0, done);
}