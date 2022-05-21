inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/daemon/bank.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_balance () {
    expect_function("query_balance", testOb);
    expect_function("update_balance", testOb);

    expect("balance handles querying and updating", (: ({
        assert(testOb->query_balance("testcharacter", "somewhere"), "==", ([ ])),
        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 123, ])),
        assert(testOb->query_balance("testcharacter", "somewhere"), "==", ([ "copper": 123, ])),
        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 54321, ])),
        assert(testOb->query_balance("testcharacter", "somewhere"), "==", ([ "copper": 54321, ])),
        assert(unguarded((: rm("/save/character/t/testcharacter/bank.o") :)), "==", 1),
    }) :));
}