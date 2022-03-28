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

void test_last_location () {
    expect_function("query_environment_path", testOb);
    expect_function("set_environment_path", testOb);

    expect("handles setting and querying last location", (: ({
        assert(testOb->query_environment_path(), "==", "/domain/Nowhere/room/void"), // defaults to void
        assert(testOb->set_environment_path("/domain/Nowhere/room/freezer"), "==", 0),
        assert(testOb->query_environment_path(), "==", "/domain/Nowhere/room/freezer"),
    }) :));

    expect("set_environment_path handles invalid argument 1", (: ({
        assert((: testOb->set_environment_path(0) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path(0.0) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path("") :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path(({})) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path(([])) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
        assert((: testOb->set_environment_path((: users :)) :), "catch", "*Bad argument 1 to move->set_environment_path\n"),
    }) :));
}