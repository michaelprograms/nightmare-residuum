inherit M_TEST;

private int action = 0;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/shell/alias.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}