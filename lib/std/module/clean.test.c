inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/clean.c");
}
void after_all_tests () {
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
        assert(member("internal_remove", functions(testOb, 0)) > -1 && !function_exists("internal_remove", testOb), "==", 1),
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

void test_clean_up () {
    expect_function("query_no_clean", testOb);
    expect_function("set_no_clean", testOb);
    expect_function("clean_up", testOb);
    expect_function("clean_never", testOb);
    expect_function("clean_later", testOb);

    expect("clean_up behaves", (: ({
        assert(testOb->clean_never(), "==", 0),
        assert(testOb->clean_later(), "==", 1),

        assert(testOb->query_no_clean(), "==", 0),
        assert(testOb->set_no_clean(1), "==", 0),
        assert(testOb->query_no_clean(), "==", 1),
        assert(testOb->clean_up(), "==", 0),
        assert(testOb->set_no_clean(0), "==", 0),
        assert(testOb->query_no_clean(), "==", 0),
        assert(objectp(testOb), "==", 1),
        assert(testOb->clean_up(), "==", 1),
        assert(objectp(testOb), "==", 0),
    }) :));
}