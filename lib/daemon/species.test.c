inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/daemon/species.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}