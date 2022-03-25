inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/container.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_receive () {
    object ob = new(STD_OBJECT);

    expect_function("can_receive", testOb);
    expect_function("can_release", testOb);
    expect_function("handle_receive", testOb);
    expect_function("handle_release", testOb);

    expect("receive and release behaves", (: ({
        assert(testOb->can_receive($(ob)), "==", 1),
        assert(testOb->can_release($(ob)), "==", 0),
        assert(testOb->handle_receive($(ob)), "==", 1),
        assert($(ob)->handle_move(testOb), "==", 1),
        assert(testOb->can_receive($(ob)), "==", 0),
        assert(testOb->can_release($(ob)), "==", 1),
        assert(testOb->handle_release($(ob)), "==", 1),
    }) :));
}

void test_inventory () {
    expect_function("inventory_visible", testOb);
    expect_function("inventory_accessible", testOb);

    expect("inventory behaves", (: ({
        assert(testOb->inventory_visible(), "==", 1),
        assert(testOb->inventory_accessible(), "==", 1),
    }) :));
}