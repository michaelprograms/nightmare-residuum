inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/levelable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_level () {
    expect_function("set_level", testOb);
    expect_function("query_level", testOb);

    expect("level settable and queryable", (: ({
        assert(testOb->query_level(), "==", 1),

        testOb->set_level(123),
        assert(testOb->query_level(), "==", 123),

        testOb->set_level(0),
        assert(testOb->query_level(), "==", 0),

        testOb->set_level(-123),
        assert(testOb->query_level(), "==", 123),
    }) :));
}