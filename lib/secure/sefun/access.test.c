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
        assert(testOb->unguarded((: MAX_INT :)), "==", MAX_INT),
        assert(testOb->unguarded(function () { return MAX_INT; }), "==", MAX_INT),
    }) :));

    expect("unguarded handled bad argument 1", (: ({
        assert((: testOb->unguarded() :), "catch", "*Bad argument 1 to access->unguarded\n"),
        assert((: testOb->unguarded(this_object()) :), "catch", "*Bad argument 1 to access->unguarded\n"),
        assert((: testOb->unguarded("") :), "catch", "*Bad argument 1 to access->unguarded\n"),
        assert((: testOb->unguarded(1) :), "catch", "*Bad argument 1 to access->unguarded\n"),
        assert((: testOb->unguarded(1.0) :), "catch", "*Bad argument 1 to access->unguarded\n"),
        assert((: testOb->unguarded(({})) :), "catch", "*Bad argument 1 to access->unguarded\n"),
        assert((: testOb->unguarded(([])) :), "catch", "*Bad argument 1 to access->unguarded\n"),
    }) :));
}