inherit M_TEST;

void test_balance () {
    expect("balance handles querying and updating", (: ({
        // test query_balance and update_balance
        assert_equal(testOb->query_balance("testcharacter", "somewhere"), ([ ])),
        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 123, ])),
        assert_equal(testOb->query_balance("testcharacter", "somewhere"), ([ "copper": 123, ])),
        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 54321, ])),
        assert_equal(testOb->query_balance("testcharacter", "somewhere"), ([ "copper": 54321, ])),
        // test query_banks
        assert_equal(testOb->query_banks("testcharacter"), ({ "somewhere", })),
        testOb->update_balance("testcharacter", "elsewhere", ([ "copper": 54321, ])),
        assert_equal(testOb->query_banks("testcharacter"), ({ "somewhere", "elsewhere", })),
        assert_equal(rm("/save/character/t/testcharacter/bank.o"), 1),
    }) :));
}