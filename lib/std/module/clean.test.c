inherit M_TEST;
inherit M_CONTAINER;

string *test_order () {
    return ({
        "test_handle_remove",
        "test_internal_remove",
        "test_clean_up",
    });
}

void test_handle_remove () {
    expect("handle_remove behaves", (: ({
        assert_equal(objectp(testOb), 1),
        assert_equal(testOb->handle_remove(), 1),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_internal_remove () {
    object storage, ob;

    expect("internal_remove is protected", (: ({
        assert_equal(member_array("internal_remove", functions(testOb, 0)) > -1 && !function_exists("internal_remove", testOb), 1),
    }) :));

    storage = new(STD_STORAGE);
    ob = new(STD_OBJECT);
    ob->handle_move(storage);

    expect("internal_remove behaves", (: ({
        assert_equal(environment($(ob)), $(storage)),
        assert_equal($(storage)->handle_remove(), 1),
        assert_equal(objectp($(ob)), 0),
    }) :));

    if (objectp(storage)) destruct(storage);
    if (objectp(ob)) destruct(ob);
}

void test_defaults () {
    expect("default clean default values", (: ({
        assert_equal(testOb->clean_never(), 0),
        assert_equal(testOb->clean_later(), 1),
    }) :));
}

void test_clean_up () {
    expect("no clean behaves", (: ({
        assert_equal(testOb->query_no_clean(), 0),
        testOb->set_no_clean(1),
        assert_equal(testOb->query_no_clean(), 1),
        assert_equal(testOb->clean_up(), 0),
        testOb->set_no_clean(0),
        assert_equal(testOb->query_no_clean(), 0),
        assert_equal(objectp(testOb), 1),
        assert_equal(testOb->clean_up(), 1),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_clean_up_item_with_environment () {
    object item = new(STD_ITEM);
    expect("clean up item with environment behaves", (: ({
        assert_equal($(item)->handle_move(this_object()), 1),
        assert_equal($(item)->clean_up(), 0),
        assert_equal($(item)->handle_remove(), 1),
    }) :));
    if (item) destruct(item);
}