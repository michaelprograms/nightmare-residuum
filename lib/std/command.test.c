inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/command.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({
        "test_name",
        "test_syntax",
        "test_help_text",
        "test_help_similar",
        "test_handle_help",
     });
}

void test_name () {
    expect_function("query_name", testOb);

    expect("handles command name", (: ({
        assert(testOb->query_name(), "==", "command"),
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

void test_handle_help () {
    expect_function("handle_help", testOb);

    expect("handles formatting help file", (: ({
        // should contain Syntax section always
        assert(regexp(testOb->handle_help(this_object()), "Syntax"), "==", 1),

        // only contain Description section if set
        assert(regexp(testOb->handle_help(this_object()), "Description"), "==", 0),
        assert(regexp(testOb->handle_help(this_object()), "Help text"), "==", 0),
        testOb->set_help_text("Help text"),
        assert(regexp(testOb->handle_help(this_object()), "Description"), "==", 1),
        assert(regexp(testOb->handle_help(this_object()), "Help text"), "==", 1),

        // only contain Similar Actions section if set
        assert(regexp(testOb->handle_help(this_object()), "Similar Actions"), "==", 0),
        assert(regexp(testOb->handle_help(this_object()), "A, B, C"), "==", 0),
        testOb->set_help_similar(({ "A", "B", "C", })),
        assert(regexp(testOb->handle_help(this_object()), "Similar Actions"), "==", 1),
        assert(regexp(testOb->handle_help(this_object()), "A, B, C"), "==", 1),
    }) :));
}