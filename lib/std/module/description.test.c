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

        testOb->set_long((: base_name(previous_object(1)) :)),
        assert(testOb->query_long(), "==", base_name()),

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

        testOb->set_short((: base_name(previous_object(1)) :)),
        assert(testOb->query_short(), "==", base_name()),

        testOb->set_short(0),
        assert(testOb->query_short(), "==", 0),
    }) :));

    expect("short handles DEFAULT ANSI replacement", (: ({
        testOb->set_short("%^BLUE%^Blue text%^DEFAULT%^"),
        assert(testOb->query_short(), "==", "%^BLUE%^Blue text%^RESET%^"),
        assert(testOb->query_short("%^RED%^"), "==", "%^BLUE%^Blue text%^RED%^"),
    }) :));
}