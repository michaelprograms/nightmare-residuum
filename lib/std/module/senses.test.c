inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/senses.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_listen () {
    expect_function("set_listen", testOb);
    expect_function("query_listen", testOb);

    expect("listens are settable and queryable", (: ({
        assert(testOb->query_listen("default"), "==", 0),
        testOb->set_listen("default", "Default sounds."),
        assert(testOb->query_listen("default"), "==", "Default sounds."),
    }) :));
}

void test_smell () {
    expect_function("set_smell", testOb);
    expect_function("query_smell", testOb);

    expect("smells are settable and queryable", (: ({
        assert(testOb->query_smell("default"), "==", 0),
        testOb->set_smell("default", "Default scents."),
        assert(testOb->query_smell("default"), "==", "Default scents."),
    }) :));
}