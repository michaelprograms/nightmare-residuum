inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

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
        assert_equal(unguarded((: rm("/save/character/t/testcharacter/bank.o") :)), 1),
    }) :));
}