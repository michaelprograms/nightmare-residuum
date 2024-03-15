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
        assert_equal(testOb->query_shadow(), UNDEFINED),
        assert_equal(testOb->start_shadow(this_object()), 1),
        assert_equal(testOb->query_shadow(), this_object()),
        assert_equal(testOb->stop_shadow(), 1),
    }) :));
}