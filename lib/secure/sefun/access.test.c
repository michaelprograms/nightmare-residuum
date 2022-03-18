inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/access.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_access () {
    expect_function("unguarded", testOb);

    expect("unguarded evaluates function", (: ({
        assert((: testOb->unguarded((: MAX_INT :)) :), "==", MAX_INT),
        assert((: testOb->unguarded(function () { return MAX_INT; }) :), "==", MAX_INT),
    }) :));

    expect_catch((: testOb->unguarded() :), "*Bad argument 1 to access->unguarded\n", "unguarded handled bad arguments");
    expect_catch((: testOb->unguarded("bad argument") :), "*Illegal unguarded.\n", "unguarded threw error");
}