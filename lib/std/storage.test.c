inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/storage.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_long () {
    object ob1 = new(STD_ITEM);
    object ob2 = new(STD_ITEM);
    object ob3 = new(STD_ITEM);

    expect_function("query_long", testOb);

    testOb->set_long("Storage.");
    ob1->set_short("a rock");
    ob2->set_short("a paper");
    ob3->set_short("a scissors");

    expect("query_long returns contents", (: ({
        assert(sizeof(testOb->query_item_contents()), "==", 0),
        assert(testOb->query_long(), "==", "Storage."),

        // 1 item
        assert($(ob1)->handle_move(testOb), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 1),
        assert(testOb->query_long(), "==", "Storage.\n\nA rock is inside."),

        // 2 items
        assert($(ob2)->handle_move(testOb), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 2),
        assert(testOb->query_long(), "==", "Storage.\n\nA paper and a rock are inside."),

        // 3 items
        assert($(ob3)->handle_move(testOb), "==", 1),
        assert(sizeof(testOb->query_item_contents()), "==", 3),
        assert(testOb->query_long(), "==", "Storage.\n\nA paper, a rock, and a scissors are inside."),

    }) :));
    destruct(ob1);
    destruct(ob2);
    destruct(ob3);
}