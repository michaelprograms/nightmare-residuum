inherit M_TEST;

#define ACCOUNT_PATH "/save/account/a/accounttest.o"

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/user/account.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
    if (unguarded((: file_size(ACCOUNT_PATH) :)) > -1) {
        unguarded((: rm(ACCOUNT_PATH) :));
    }
}

void test_account_name () {
    expect_function("query_name", testOb);
    expect_function("query_key_name", testOb);
    expect_function("load_account", testOb);

    expect("account name is settable and queryable", (: ({
        assert(testOb->query_name(), "==", 0),
        testOb->load_account("accounttest"),
        assert(testOb->query_name(), "==", "accounttest"),
        assert(testOb->query_key_name(), "==", "accounttest"),
        assert(testOb->query_save_path(), "==", ACCOUNT_PATH),
        testOb->load_account("AccountTest"),
        assert(testOb->query_name(), "==", "AccountTest"),
        assert(testOb->query_key_name(), "==", "accounttest"),
        assert(testOb->query_save_path(), "==", ACCOUNT_PATH),
    }) :));
}