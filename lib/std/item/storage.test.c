inherit M_TEST;
inherit M_MOVE;
inherit M_CONTAINER;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/item/storage.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_long () {
    object ob1 = new(STD_ITEM);
    object ob2 = new(STD_ITEM);
    object ob3 = new(STD_ITEM);

    expect_function("query_long", testOb);

    testOb->set_long("Storage.");
    ob1->set_short("a rock");
    ob2->set_short("a paper");
    ob3->set_short("a scissors");

    expect("query_long returns contents", (: ({
        assert(sizeof(testOb->query_item_contents()), "==", 0),
        assert(testOb->query_long(), "==", "Storage."),

        // 1 item
        assert($(ob1)->handle_move(testOb), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 1),
        assert(testOb->query_long(), "==", "Storage.\n\nA rock is inside."),

        // 2 items
        assert($(ob2)->handle_move(testOb), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 2),
        assert(testOb->query_long(), "==", "Storage.\n\nA paper and a rock are inside."),

        // 3 items
        assert($(ob3)->handle_move(testOb), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 3),
        assert(testOb->query_long(), "==", "Storage.\n\nA paper, a rock, and a scissors are inside."),

    }) :));
    destruct(ob1);
    destruct(ob2);
    destruct(ob3);
}

void test_apply_get_obj_from_obj () {
    object storage1, storage2;

    expect_function("indirect_get_obj_from_obj", testOb);

    storage1 = new(STD_STORAGE);
    storage2 = new(STD_STORAGE);
    expect("indirect_get_obj_from_obj should behave", (: ({
        // same environment, success
        assert(this_object()->handle_move($(storage1)), "==", 1),
        assert(testOb->handle_move($(storage1)), "==", 1),
        assert(testOb->indirect_get_obj_from_obj(), "==", 1),
        // different environments, fail
        assert(testOb->handle_move($(storage2)), "==", 1),
        assert(testOb->indirect_get_obj_from_obj(), "==", "You can't get what isn't here."),
        // in our inventory, silent fail
        assert(testOb->handle_move(this_object()), "==", 1),
        assert(testOb->indirect_get_obj_from_obj(), "==", 0),
        // move test items to void
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
    }) :));

    destruct(storage1);
    destruct(storage2);
}

void test_apply_put_obj_in_obj () {
    object ob;

    expect_function("indirect_put_obj_in_obj", testOb);

    ob = new(STD_ITEM);

    expect("indirect_put_obj_in_obj should behave", (: ({
        // test bad inputs
        assert(testOb->indirect_put_obj_in_obj(), "==", 0),
        assert(testOb->indirect_put_obj_in_obj(({ })), "==", 0),
        assert(testOb->indirect_put_obj_in_obj(0), "==", 0),
        // different container, fail
        assert(testOb->indirect_put_obj_in_obj($(ob)), "==", "You can't put what you don't have."),
        // obj is in inventory
        assert($(ob)->handle_move(this_object()), "==", 1),
        assert(testOb->indirect_put_obj_in_obj($(ob)), "==", 1),
        // move test items to void
        assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        assert($(ob)->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
    }) :));

    destruct(ob);
}