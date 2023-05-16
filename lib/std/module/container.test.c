inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/container.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_receive () {
    object ob = new(STD_OBJECT);

    expect_function("can_receive", testOb);
    expect_function("can_release", testOb);
    expect_function("handle_receive", testOb);
    expect_function("handle_release", testOb);

    expect("receive and release behaves", (: ({
        assert(testOb->can_receive($(ob)), "==", 1),
        assert(testOb->can_release($(ob)), "==", 0),
        assert(testOb->handle_receive($(ob)), "==", 1),
        assert($(ob)->handle_move(testOb), "==", 1),
        assert(testOb->can_receive($(ob)), "==", 0),
        assert(testOb->can_release($(ob)), "==", 1),
        assert(testOb->handle_release($(ob)), "==", 1),
    }) :));
}

void test_inventory () {
    expect_function("inventory_visible", testOb);
    expect_function("inventory_accessible", testOb);

    expect("inventory behaves", (: ({
        assert(testOb->inventory_visible(), "==", 1),
        assert(testOb->inventory_accessible(), "==", 1),
    }) :));
}

void test_query_contents () {
    object living, item;

    expect_function("query_living_contents", testOb);
    expect_function("query_item_contents", testOb);
    expect_function("query_contents", testOb);

    // create test items
    living = new(STD_LIVING);
    item = new(STD_ITEM);

    expect("query filtered contents behave", (: ({
        // verify empty
        assert(sizeof(testOb->query_living_contents()), "==", 0),
        assert(sizeof(testOb->query_item_contents()), "==", 0),
        assert(sizeof(testOb->query_contents()), "==", 0),
        // move test items
        assert($(living)->handle_move(testOb), "==", 1),
        assert($(item)->handle_move(testOb), "==", 1),
        // verify contents
        assert(sizeof(testOb->query_living_contents()), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 1),
        assert(sizeof(testOb->query_contents()), "==", 2),
    }) :));

    if (living) destruct(living);
    if (item) destruct(item);
}