inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/description.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_long () {
    expect_function("set_long", testOb);
    expect_function("query_long", testOb);

    expect("long is settable and queryable", (: ({
        assert(testOb->query_long(), "==", UNDEFINED),

        testOb->set_long("Test long"),
        assert(testOb->query_long(), "==", "Test long"),

        testOb->set_long((: object_name(previous_object(1)) :)),
        assert(testOb->query_long(), "==", object_name(this_object())),

        testOb->set_long(0),
        assert(testOb->query_long(), "==", 0),
    }) :));
}

void test_short () {
    expect_function("set_short", testOb);
    expect_function("query_short", testOb);

    expect("short is settable and queryable", (: ({
        assert(testOb->query_short(), "==", UNDEFINED),

        testOb->set_short("Test short"),
        assert(testOb->query_short(), "==", "Test short"),

        testOb->set_short((: object_name(previous_object(1)) :)),
        assert(testOb->query_short(), "==", object_name(this_object())),

        testOb->set_short(0),
        assert(testOb->query_short(), "==", 0),
    }) :));
}