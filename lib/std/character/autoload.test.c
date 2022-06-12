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
    object ob1, ob2;

    expect_function("update_autoload", testOb);
    expect_function("query_autoload_items", testOb);
    expect_function("restore_autoload", testOb);

    // set need set_level
    destruct(testOb);
    testOb = new("/std/character.c");
    testOb->set_level(1);

    ob1 = new(STD_ITEM);
    ob1->set_autoload(1);
    ob2 = new(STD_ITEM);
    ob2->set_autoload(0);

    expect("autoload updates and restores", (: ({
        // setup
        assert(testOb->query_level(), "==", 1),

        // move item to inventory
        assert($(ob1)->handle_move(testOb), "==", 1),
        assert($(ob2)->handle_move(testOb), "==", 1),
        assert(sizeof(all_inventory(testOb)), "==", 2),
        assert(sizeof(testOb->query_autoload_items()), "==", 0),

        // update autoload without exit flag
        testOb->update_autoload(),
        // one item autoloads
        assert(sizeof(testOb->query_autoload_items()), "==", 1),
        // all inventory remains
        assert(objectp($(ob1)), "==", 1),
        assert(objectp($(ob2)), "==", 1),

        // update autoload with exit flag
        testOb->update_autoload(1),
        // only non-autoload inventory remains
        assert(sizeof(testOb->query_autoload_items()), "==", 1),
        assert(sizeof(all_inventory(testOb)), "==", 1),
        assert(objectp($(ob1)), "==", 0),
        assert(objectp($(ob2)), "==", 1),
        assert($(ob2)->handle_remove(), "==", 1),

        // verify inventory cleaned
        assert(sizeof(all_inventory(testOb)), "==", 0),
        assert(objectp($(ob1)), "==", 0),
        assert(objectp($(ob2)), "==", 0),

        // restore autoload
        testOb->restore_autoload(),
        // item remains in autoload array
        assert(sizeof(testOb->query_autoload_items()), "==", 1),
        // inventory restored
        assert(sizeof(all_inventory(testOb)), "==", 1),
        assert(base_name(all_inventory(testOb)[0]), "==", "/std/item"),
    }) :));

    if (ob1) destruct(ob1);
    if (ob2) destruct(ob2);
}