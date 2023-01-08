inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/command.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_name () {
    expect_function("query_name", testOb);

    expect("handles command name", (: ({
        assert(testOb->query_name(), "==", "command"),
    }) :));
}

void test_help_text () {
    expect_function("query_help_text", testOb);
    expect_function("set_help_text", testOb);

    expect("handles help text", (: ({
        assert(testOb->query_help_text(), "==", UNDEFINED),

        testOb->set_help_text("Help text."),
        assert(testOb->query_help_text(), "==", "Help text."),

        testOb->set_help_text("Different help text."),
        assert(testOb->query_help_text(), "==", "Different help text."),
    }) :));
}

void test_help_similar () {
    expect_function("query_help_similar", testOb);
    expect_function("set_help_similar", testOb);

    expect("handles help similar", (: ({
        assert(testOb->query_help_similar(), "==", UNDEFINED),

        testOb->set_help_similar(({ "a", "b", "c", })),
        assert(testOb->query_help_similar(), "==", ({ "a", "b", "c", })),

        testOb->set_help_similar(({ "1", "2", "3", })),
        assert(testOb->query_help_similar(), "==", ({ "1", "2", "3", })),
    }) :));
}

void test_syntax () {
    expect_function("query_syntax", testOb);
    expect_function("set_syntax", testOb);

    expect("handles setting and querying syntax", (: ({
        assert(testOb->query_syntax(), "==", UNDEFINED),

        testOb->set_syntax(testOb->query_name()),
        assert(testOb->query_syntax(), "==", "<" + testOb->query_name() + ">"),

        testOb->set_syntax(testOb->query_name() + " [target]"),
        assert(testOb->query_syntax(), "==", "<" + testOb->query_name() + " [target]>"),
    }) :));
}