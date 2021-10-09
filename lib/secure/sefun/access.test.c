inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/access");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_access () {
    mixed err;

    expect_function("unguarded", testOb);
    err = catch (testOb->unguarded());
    expect_strings_equal(err, "*Illegal unguarded.\n", "unguarded threw error");
    expect_true(testOb->unguarded(function () { return MAX_INT; }) == MAX_INT, "unguarded evaluated function");
}