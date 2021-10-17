inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/access");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_access () {

    expect_function("unguarded", testOb);

    expect_catch ((: testOb->unguarded() :), "*Illegal unguarded.\n", "unguarded threw error");

    expect_true(testOb->unguarded(function () { return MAX_INT; }) == MAX_INT, "unguarded evaluated function");
}