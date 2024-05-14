inherit M_TEST;

void test_banks () {
    expect("banks are queryable", (: ({
        assert_equal(testOb->query_banks("testcharacter"), ({ })),

        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 123, ])),
        assert_equal(testOb->query_banks("testcharacter"), ({ "somewhere", })),

        testOb->update_balance("testcharacter", "elsewhere", ([ "copper": 54321, ])),
        assert_equal(testOb->query_banks("testcharacter"), ({ "somewhere", "elsewhere", })),

        assert_equal(rm("/save/character/t/testcharacter/bank.o"), 1),
        assert_equal(testOb->query_banks("testcharacter"), ({ })),
    }) :));
}

void test_balance () {
    expect("balance handles querying and updating", (: ({
        assert_equal(testOb->query_balance("testcharacter", "somewhere"), ([ ])),

        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 123, ])),
        assert_equal(testOb->query_balance("testcharacter", "somewhere"), ([ "copper": 123, ])),

        testOb->update_balance("testcharacter", "somewhere", ([ "copper": 54321, ])),
        assert_equal(testOb->query_balance("testcharacter", "somewhere"), ([ "copper": 54321, ])),

        assert_equal(rm("/save/character/t/testcharacter/bank.o"), 1),
    }) :));

    expect("updating balance handles bad arguments", (: ({
        assert_catch((: testOb->update_balance(UNDEFINED, UNDEFINED, UNDEFINED) :), "*Bad argument 1 to bank->update_balance\n"),
        assert_catch((: testOb->update_balance("123", UNDEFINED, UNDEFINED) :), "*Bad argument 1 to bank->update_balance\n"),

        assert_catch((: testOb->update_balance("testcharacter", UNDEFINED, UNDEFINED) :), "*Bad argument 2 to bank->update_balance\n"),
        assert_catch((: testOb->update_balance("testcharacter", 0, UNDEFINED) :), "*Bad argument 2 to bank->update_balance\n"),

        assert_catch((: testOb->update_balance("testcharacter", "somewhere", UNDEFINED) :), "*Bad argument 3 to bank->update_balance\n"),
    }) :));

    expect("querying balance handles bad arguments", (: ({
        assert_catch((: testOb->query_balance(UNDEFINED, UNDEFINED, UNDEFINED) :), "*Bad argument 1 to bank->query_balance\n"),
        assert_catch((: testOb->query_balance("123", UNDEFINED, UNDEFINED) :), "*Bad argument 1 to bank->query_balance\n"),

        assert_catch((: testOb->query_balance("testcharacter", UNDEFINED, UNDEFINED) :), "*Bad argument 2 to bank->query_balance\n"),
        assert_catch((: testOb->query_balance("testcharacter", 0, UNDEFINED) :), "*Bad argument 2 to bank->query_balance\n"),
    }) :));


}