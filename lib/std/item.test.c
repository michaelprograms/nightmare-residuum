inherit M_TEST;
inherit STD_CONTAINER;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item.c");
    this_object()->handle_move("/domain/Nowhere/room/void.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_is_item () {
    expect_function("is_item", testOb);

    expect("is_item returns true", (: ({
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_weapon(), "==", 0),
        assert(testOb->is_food(), "==", 0),
    }) :));
}

void test_item_verb_look_applies () {
    object r = new(STD_ROOM);

    expect_function("direct_look_at_obj", testOb);
    expect_function("direct_look_obj", testOb);

    expect("item handles verb apply direct_look_at_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(environment(), "==", find_object("/domain/Nowhere/room/void.c")),

        assert(testOb->direct_look_at_obj(), "==", 0),
        assert(testOb->direct_look_obj(), "==", 0),

        assert(testOb->handle_move($(r)), "==", 1),
        assert(environment(testOb), "==", $(r)),
        assert(this_object()->handle_move($(r)), "==", 1),
        assert(environment(), "==", $(r)),

        assert(testOb->direct_look_at_obj(), "==", 1),
        assert(testOb->direct_look_obj(), "==", 1),

        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
    }) :));

    if (r) destruct(r);
}

void test_item_verb_drop_applies () {
    expect_function("direct_drop_obj", testOb);

    expect("item handles verb apply direct_drop_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(environment(this_object()), "==", find_object("/domain/Nowhere/room/void.c")),
        assert(testOb->direct_drop_obj(), "==", 0), // can't drop item
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(environment(testOb), "==", this_object()),
        assert(testOb->direct_drop_obj(), "==", 1), // can drop
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
    }) :));
}

void test_item_verb_give_applies () {
    expect_function("direct_give_obj_to_liv", testOb);

    expect("item handled verb apply direct_give_obj_to_liv", (: ({
        assert(environment(testOb), "==", 0),
        assert(testOb->direct_give_obj_to_liv(), "==", 0), // can't give item yet
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(environment(testOb), "==", this_object()),
        assert(testOb->direct_give_obj_to_liv(), "==", 1), // can give item
    }) :));
}

void test_item_verb_get_applies () {
    expect_function("direct_get_obj", testOb);

    expect("item handled verb apply direct_get_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(environment(this_object()), "==", find_object("/domain/Nowhere/room/void.c")),
        assert(testOb->direct_get_obj(), "==", "You can't get what isn't here."),
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(environment(testOb), "==", find_object("/domain/Nowhere/room/void.c")),
        assert(testOb->direct_get_obj(), "==", 1), // can get item
        assert(testOb->handle_move(this_object()), "==", 1), // move item back
        assert(testOb->direct_get_obj(), "==", 0), // already has item get item
    }) :));
}