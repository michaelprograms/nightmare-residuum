inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/resource/resource.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("type is settable and queryable", (: ({
        assert_equal(testOb->query_type(), UNDEFINED),

        testOb->set_type("test"),
        assert_equal(testOb->query_type(), "test"),

        testOb->set_type("test2"),
        assert_equal(testOb->query_type(), "test2"),
    }) :));
}