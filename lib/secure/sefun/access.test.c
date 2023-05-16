inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/access.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_access () {
    expect_function("unguarded", testOb);

    expect("unguarded evaluates function", (: ({
        assert(testOb->unguarded((: MAX_INT :)), "==", MAX_INT),
        assert(testOb->unguarded(function () { return MAX_INT; }), "==", MAX_INT),
    }) :));
}