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
        // verify initial default
        assert(testOb->query_listen("default"), "==", UNDEFINED),
        // verify default
        testOb->set_listen("default", "Default sounds."),
        assert(testOb->query_listen("default"), "==", "Default sounds."),
        // verify non-default
        testOb->set_listen("something", "Some noise."),
        assert(testOb->query_listen("something"), "==", "Some noise."),
        // verify function
        testOb->set_listen("fn", function () { return "Function sound."; }),
        assert(testOb->query_listen("fn"), "==", function () { return "Function sound."; }),
    }) :));
}

void test_smell () {
    expect_function("set_smell", testOb);
    expect_function("query_smell", testOb);

    expect("smells are settable and queryable", (: ({
        // verify initial default
        assert(testOb->query_smell("default"), "==", 0),
        // verify default
        testOb->set_smell("default", "Default scents."),
        assert(testOb->query_smell("default"), "==", "Default scents."),
        // verify non-default
        testOb->set_smell("something", "Some smell."),
        assert(testOb->query_smell("something"), "==", "Some smell."),
        // verify function
        testOb->set_smell("fn", function () { return "Function scent."; }),
        assert(testOb->query_smell("fn"), "==", function () { return "Function scent."; }),
    }) :));
}