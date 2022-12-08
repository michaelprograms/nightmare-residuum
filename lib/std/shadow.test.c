inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/shadow.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_shadow () {
    expect_function("start_shadow", testOb);
    expect_function("stop_shadow", testOb);
    expect_function("query_shadow", testOb);

    expect("shadow handled starting and stopping", (: ({
        assert(testOb->query_shadow(), "==", UNDEFINED),
        assert(testOb->start_shadow(this_object()), "==", 1),
        assert(testOb->query_shadow(), "==", this_object()),
        assert(testOb->stop_shadow(), "==", 1),
    }) :));
}