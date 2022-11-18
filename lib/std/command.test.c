#include <verb.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/verb.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
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

    expect("set_help_text handles invalid argument 1", (: ({
        assert((: testOb->set_help_text(this_object()) :), "catch", "*Bad argument 1 to command->set_help_text\n"),
        assert((: testOb->set_help_text(1.0) :), "catch", "*Bad argument 1 to command->set_help_text\n"),
        assert((: testOb->set_help_text(1) :), "catch", "*Bad argument 1 to command->set_help_text\n"),
        assert((: testOb->set_help_text(({})) :), "catch", "*Bad argument 1 to command->set_help_text\n"),
        assert((: testOb->set_help_text(([])) :), "catch", "*Bad argument 1 to command->set_help_text\n"),
        assert((: testOb->set_help_text((: 1 :)) :), "catch", "*Bad argument 1 to command->set_help_text\n"),
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

    expect("set_syntax handles invalid argument 1", (: ({
        assert((: testOb->set_syntax(this_object()) :), "catch", "*Bad argument 1 to command->set_syntax\n"),
        assert((: testOb->set_syntax(1.0) :), "catch", "*Bad argument 1 to command->set_syntax\n"),
        assert((: testOb->set_syntax(1) :), "catch", "*Bad argument 1 to command->set_syntax\n"),
        assert((: testOb->set_syntax(({})) :), "catch", "*Bad argument 1 to command->set_syntax\n"),
        assert((: testOb->set_syntax(([])) :), "catch", "*Bad argument 1 to command->set_syntax\n"),
        assert((: testOb->set_syntax((: 1 :)) :), "catch", "*Bad argument 1 to command->set_syntax\n"),
    }) :));
}