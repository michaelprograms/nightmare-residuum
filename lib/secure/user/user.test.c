inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/user/user");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_applies () {
    expect_function("logon", testOb);
    expect_function("net_dead", testOb);
    expect_function("receive_message", testOb);
    expect_function("reconnect", testOb);
    expect_function("quit_character", testOb);
    expect_function("quit_account", testOb);
}