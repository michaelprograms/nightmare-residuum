inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/user/account.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}