inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/npc/vendor.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_npc () {
    expect_function("is_vendor", testOb);

    expect("is_vendor behaves", (: ({
        assert(testOb->is_living(), "==", 1),
        assert(testOb->is_vendor(), "==", 1),
        assert(testOb->is_npc(), "==", 1),
        assert(testOb->is_monster(), "==", UNDEFINED),
    }) :));
}