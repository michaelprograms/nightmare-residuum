inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/user.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}