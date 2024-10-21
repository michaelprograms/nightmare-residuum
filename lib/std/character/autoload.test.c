inherit M_TEST;

void test_autoload () {
    object mockBody = new("/std/living/body.mock.c");
    object mockItem1 = new("/std/item.mock.c");
    object ob1, ob2, ob3;

    ob1 = new(STD_ITEM);
    ob1->set_autoload(1);
    ob2 = new(STD_ITEM);
    ob2->set_autoload(1);
    ob3 = new(STD_ITEM);
    ob3->set_autoload(0);

    mockBody->start_shadow(testOb);
    testOb->set_level(1);
    ob1->handle_move(testOb);
    ob2->handle_move(testOb);
    ob3->handle_move(testOb);
    mockItem1->start_shadow(ob1);
    ob1->set_autoload_data(({ "test", "123", "abc" }));

    expect("autoload updates and restores", (: ({
        assert_equal(testOb->query_level(), 1),
        assert_equal(sizeof(all_inventory(testOb)), 3),
        assert_equal(sizeof(testOb->query_autoload_items()), 0),

        // update autoload without exit flag
        testOb->update_autoload(),
        // one item autoloads
        assert_equal(sizeof(testOb->query_autoload_items()), 2),
        // all inventory remains
        assert_equal(objectp($(ob1)), 1),
        assert_equal(objectp($(ob3)), 1),

        // update autoload with exit flag
        testOb->update_autoload(1),
        // only non-autoload inventory remains
        assert_equal(sizeof(testOb->query_autoload_items()), 2),
        assert_equal(sizeof(all_inventory(testOb)), 1),
        assert_equal(objectp($(ob1)), 0),
        assert_equal(objectp($(ob3)), 1),
        assert_equal($(ob3)->handle_remove(), 1),

        // verify inventory cleaned
        assert_equal(sizeof(all_inventory(testOb)), 0),
        assert_equal(objectp($(ob1)), 0),
        assert_equal(objectp($(ob2)), 0),
        assert_equal(objectp($(ob3)), 0),

        // restore autoload
        testOb->restore_autoload(),
        // item remains in autoload array
        assert_equal(sizeof(testOb->query_autoload_items()), 2),
        // inventory restored
        assert_equal(sizeof(all_inventory(testOb)), 2),
        assert_equal(base_name(all_inventory(testOb)[0]), "/std/item"),
    }) :));

    mockBody->stop_shadow();
    if (ob1) destruct(ob1);
    if (ob2) destruct(ob2);
    if (ob3) destruct(ob3);
    if (mockBody) destruct(mockBody);
    if (mockItem1) destruct(mockItem1);
}