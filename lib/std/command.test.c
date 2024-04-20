inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
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
    expect("handles command name", (: ({
        assert_equal(testOb->query_name(), "command.coverage"),
    }) :));
}

void test_syntax () {
    expect("handles setting and querying syntax", (: ({
        assert_equal(testOb->query_syntax(), UNDEFINED),

        testOb->set_syntax(testOb->query_name()),
        assert_equal(testOb->query_syntax(), "<" + testOb->query_name() + ">"),

        testOb->set_syntax(testOb->query_name() + " [target]"),
        assert_equal(testOb->query_syntax(), "<" + testOb->query_name() + " [target]>"),
    }) :));
}

void test_help_text () {
    expect("handles help text", (: ({
        assert_equal(testOb->query_help_text(), UNDEFINED),

        testOb->set_help_text("Help text."),
        assert_equal(testOb->query_help_text(), "Help text."),

        testOb->set_help_text("Different help text."),
        assert_equal(testOb->query_help_text(), "Different help text."),
    }) :));
}

void test_help_similar () {
    expect("handles help similar", (: ({
        assert_equal(testOb->query_help_similar(), UNDEFINED),

        testOb->set_help_similar(({ "a", "b", "c", })),
        assert_equal(testOb->query_help_similar(), ({ "a", "b", "c", })),

        testOb->set_help_similar(({ "1", "2", "3", })),
        assert_equal(testOb->query_help_similar(), ({ "1", "2", "3", })),
    }) :));
}

void test_handle_help () {
    expect("handles formatting help file", (: ({
        // should contain Syntax section always
        assert_equal(regexp(testOb->handle_help(this_object()), "Syntax"), 1),

        // only contain Description section if set
        assert_equal(regexp(testOb->handle_help(this_object()), "Description"), 0),
        assert_equal(regexp(testOb->handle_help(this_object()), "Help text"), 0),
        testOb->set_help_text("Help text"),
        assert_equal(regexp(testOb->handle_help(this_object()), "Description"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "Help text"), 1),

        // only contain Similar Actions section if set
        assert_equal(regexp(testOb->handle_help(this_object()), "Similar Actions"), 0),
        assert_equal(regexp(testOb->handle_help(this_object()), "A, B, C"), 0),
        testOb->set_help_similar(({ "A", "B", "C", })),
        assert_equal(regexp(testOb->handle_help(this_object()), "Similar Actions"), 1),
        assert_equal(regexp(testOb->handle_help(this_object()), "A, B, C"), 1),
    }) :));
}