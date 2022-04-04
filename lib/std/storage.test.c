inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/storage.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}