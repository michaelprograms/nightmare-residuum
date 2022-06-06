inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/npc/vendor.c");
}
void after_each_test () {
    // vendor handle_remove to destruct vendor inventory
    if (objectp(testOb)) testOb->handle_remove();
    if (objectp(testOb)) destruct(testOb);
}

void test_vendor () {
    expect_function("is_vendor", testOb);

    expect("is_vendor behaves", (: ({
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_vendor(), "==", 1),
        assert(testOb->is_npc(), "==", 1),
        assert(testOb->is_monster(), "==", UNDEFINED),
    }) :));
}

void test_vendor_inventory () {
    object vi, ob;

    expect_function("handle_remove", testOb);
    expect_function("query_vendor_inventory", testOb);

    // grab reference to vendor inventory
    vi = testOb->query_vendor_inventory();
    // setup a test item
    ob = new(STD_ITEM);
    expect("vendor inventory is created and cleaned", (: ({
        // vendor inventory exists
        assert(objectp($(vi)), "==", 1),
        // move test item to vendor inventory
        assert($(ob)->handle_move($(vi)), "==", 1),
        // remove vendor
        assert(testOb->handle_remove(), "==", 1),
        // verify vendor inventory is destructed
        assert(objectp($(vi)), "==", 0),
        // verify item is destructed
        assert(objectp($(ob)), "==", 0),
    }) :));

    if (ob) destruct(ob);
}

void test_max_items () {
    object vi;

    expect_function("set_max_items", testOb);

    // grab reference to vendor inventory
    vi = testOb->query_vendor_inventory();
    expect("vendor inventory max items behaves", (: ({
        // verify default max items
        assert($(vi)->query_max_items(), "==", 0),
        // verify changing max items
        testOb->set_max_items(10),
        assert($(vi)->query_max_items(), "==", 10),
        // verify changing max items
        testOb->set_max_items(123),
        assert($(vi)->query_max_items(), "==", 123),
    }) :));
}