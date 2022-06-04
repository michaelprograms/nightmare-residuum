inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/character/autoload.c");
    destruct(testOb);
    testOb = new(STD_CHARACTER); // need inventory and levels
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_autoload () {
    object ob;

    expect_function("update_autoload", testOb);
    expect_function("query_autoload_items", testOb);
    expect_function("restore_autoload", testOb);

    ob = new(STD_ITEM);
    ob->set_autoload(0);
    testOb->set_level(1);
    expect("autoload updates and restores", (: ({
        // setup
        assert(testOb->query_level(), "==", 1),
        // move item to inventory
        assert($(ob)->handle_move(testOb), "==", 1),

        assert(sizeof(all_inventory(testOb)), "==", 1),
        assert(sizeof(testOb->query_autoload_items()), "==", 0),

        // update autoload
        testOb->update_autoload(),
        assert(sizeof(testOb->query_autoload_items()), "==", 1),
        assert(sizeof(all_inventory(testOb)), "==", 0),
        assert(objectp($(ob)), "==", 0),

        // restore autoload
        testOb->restore_autoload(),
        // item remains for now
        assert(sizeof(testOb->query_autoload_items()), "==", 1),
        // inventory restored
        assert(sizeof(all_inventory(testOb)), "==", 1),
    }) :));
}