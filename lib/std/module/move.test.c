inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/move.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
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