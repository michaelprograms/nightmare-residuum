inherit M_TEST;
inherit M_CONTAINER;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/clean.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_handle_remove", "test_internal_remove", "test_clean_up" });
}

void test_handle_remove () {
    expect_function("handle_remove", testOb);

    expect("handle_remove behaves", (: ({
        assert(objectp(testOb), "==", 1),
        assert(testOb->handle_remove(), "==", 1),
        assert(objectp(testOb), "==", 0),
    }) :));
}

void test_internal_remove () {
    object storage, ob;

    expect("internal_remove is protected", (: ({
        assert(member_array("internal_remove", functions(testOb, 0)) > -1 && !function_exists("internal_remove", testOb), "==", 1),
    }) :));

    storage = new(STD_STORAGE);
    ob = new(STD_OBJECT);
    ob->handle_move(storage);

    expect("internal_remove behaves", (: ({
        assert(environment($(ob)), "==", $(storage)),
        assert($(storage)->handle_remove(), "==", 1),
        assert(objectp($(ob)), "==", 0),
    }) :));

    if (objectp(storage)) destruct(storage);
    if (objectp(ob)) destruct(ob);
}

void test_defaults () {
    expect_function("clean_never", testOb);
    expect_function("clean_later", testOb);

    expect("default clean default values", (: ({
        assert(testOb->clean_never(), "==", 0),
        assert(testOb->clean_later(), "==", 1),
    }) :));
}

void test_clean_up () {
    expect_function("query_no_clean", testOb);
    expect_function("set_no_clean", testOb);
    expect_function("clean_up", testOb);

    expect("no clean behaves", (: ({
        assert(testOb->query_no_clean(), "==", 0),
        testOb->set_no_clean(1),
        assert(testOb->query_no_clean(), "==", 1),
        assert(testOb->clean_up(), "==", 0),
        testOb->set_no_clean(0),
        assert(testOb->query_no_clean(), "==", 0),
        assert(objectp(testOb), "==", 1),
        assert(testOb->clean_up(), "==", 1),
        assert(objectp(testOb), "==", 0),
    }) :));
}
void test_clean_up_with_environment () {
    object item = new(STD_ITEM);
    expect("clean up with environment behaves", (: ({
        assert($(item)->handle_move(this_object()), "==", 1),
        assert($(item)->clean_up(), "==", 0),
        assert($(item)->handle_remove(), "==", 1),
    }) :));
}

void test_clean_up_with_parent () {
    object vi = new("/std/vendor_inventory.c");

    expect("clean up with parent behaves", (: ({
        assert($(vi)->query_parent(), "==", this_object()),
        $(vi)->set_no_clean(0),
        assert($(vi)->query_no_clean(), "==", 0),
        assert($(vi)->clean_up(), "==", 1),
        assert($(vi)->handle_remove(), "==", 1),
    }) :));

}