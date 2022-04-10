inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/item/coins.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_currency () {
    expect_function("is_currency", testOb);

    expect("is_currency behaves", (: ({
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_currency(), "==", 1),
        assert(testOb->is_character(), "==", UNDEFINED),
        assert(testOb->is_monster(), "==", UNDEFINED),
        assert(testOb->is_npc(), "==", UNDEFINED),
    }) :));
}