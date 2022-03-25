inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/object.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_lifecycle_functions () {
    // @TODO test these better

    expect_function("create", testOb);

    // expect_function("heart_beat", testOb);

    expect_function("id", testOb);
    // Tests for id handled by /std/module/id.test.c

    // expect_function("move_or_destruct", testOb);

    expect_function("reset", testOb);

    // expect_function("virtual_start", testOb);
}

void test_move () {
    object ob = new(STD_CONTAINER), ob2 = new(STD_CONTAINER);

    expect_function("handle_move", testOb);

    expect("handle_move relocates objects", (: ({
        assert(sizeof(all_inventory($(ob))), "==", 0),
        assert(testOb->handle_move($(ob)), "==", 1),
        assert(sizeof(all_inventory($(ob))), "==", 1),
        assert(sizeof(all_inventory($(ob2))), "==", 0),
        assert(testOb->handle_move($(ob2)), "==", 1),
        assert(sizeof(all_inventory($(ob2))), "==", 1),
        assert(sizeof(all_inventory($(ob))), "==", 0),
    }) :));

    destruct(ob);
    destruct(ob2);
}

void test_parser_applies () {
    expect_function("is_living", testOb);
    expect_function("inventory_visible", testOb);
    expect_function("inventory_accessible", testOb);

    expect("object handles parser applies", (: ({
        assert(testOb->is_living(), "==", 0),
        assert(testOb->inventory_visible(), "==", 0),
        assert(testOb->inventory_accessible(), "==", 0),
    }) :));
}