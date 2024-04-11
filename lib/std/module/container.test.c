inherit M_TEST;

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

void test_receive () {
    object ob = new(STD_OBJECT);

    expect("receive and release behaves", (: ({
        assert_equal(testOb->can_receive($(ob)), 1),
        assert_equal(testOb->can_release($(ob)), 0),
        assert_equal(testOb->handle_receive($(ob)), 1),
        assert_equal($(ob)->handle_move(testOb), 1),
        assert_equal(testOb->can_receive($(ob)), 0),
        assert_equal(testOb->can_release($(ob)), 1),
        assert_equal(testOb->handle_release($(ob)), 1),
    }) :));
}

void test_inventory () {
    expect("inventory behaves", (: ({
        assert_equal(testOb->inventory_visible(), 1),
        assert_equal(testOb->inventory_accessible(), 1),
    }) :));
}

void test_query_contents () {
    object living, item;

    // create test items
    living = new(STD_LIVING);
    item = new(STD_ITEM);

    expect("query filtered contents behave", (: ({
        // verify empty
        assert_equal(sizeof(testOb->query_living_contents()), 0),
        assert_equal(sizeof(testOb->query_item_contents()), 0),
        assert_equal(sizeof(testOb->query_contents()), 0),
        // move test items
        assert_equal($(living)->handle_move(testOb), 1),
        assert_equal($(item)->handle_move(testOb), 1),
        // verify contents
        assert_equal(sizeof(testOb->query_living_contents()), 1),
        assert_equal(sizeof(testOb->query_item_contents()), 1),
        assert_equal(sizeof(testOb->query_contents()), 2),
    }) :));
}