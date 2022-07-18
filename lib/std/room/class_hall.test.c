inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/room/class_hall.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_class () {
    expect_function("query_class", testOb);
    expect_function("set_class", testOb);

    expect("class is queryable and settable", (: ({
        assert(testOb->query_class(), "==", 0),

        testOb->set_class("warrior"),
        assert(testOb->query_class(), "==", "warrior"),

        testOb->set_class("templar"),
        assert(testOb->query_class(), "==", "templar"),
    }) :));
}