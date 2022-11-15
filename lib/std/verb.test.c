#include <verb.h>

inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/verb.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

object __MockCharacter;
object query_character () {
    return __MockCharacter;
}

void test_name () {
    expect_function("query_name", testOb);

    expect("handles verb name", (: ({
        assert(testOb->query_name(), "==", "verb"),
    }) :));
}

void test_requirements () {
    expect_function("set_requirements", testOb);
    expect_function("query_requirements", testOb);
    expect_function("can_verb_rule", testOb);
    expect_function("check_busy", testOb);
    expect_function("check_disable", testOb);

    expect("requirements should be settable and queryable", (: ({
        assert(testOb->query_requirements(), "==", REQUIREMENT_NONE),

        testOb->set_requirements(REQUIREMENT_BUSY),
        assert(testOb->query_requirements(), "==", REQUIREMENT_BUSY),

        testOb->set_requirements(REQUIREMENT_DISABLE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_DISABLE),

        testOb->set_requirements(REQUIREMENT_BUSY & REQUIREMENT_DISABLE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_BUSY & REQUIREMENT_DISABLE),
    }) :));

    __MockCharacter = new("/std/character.c");
    __MockCharacter->set_key_name("tester");
    expect("can_verb_rule should use requirements", (: ({
        // test singular requirements
        testOb->set_requirements(REQUIREMENT_NONE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_NONE),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1), // passes

        testOb->set_requirements(REQUIREMENT_BUSY),
        assert(testOb->query_requirements(), "==", REQUIREMENT_BUSY),
        assert(testOb->check_busy(), "==", 1),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),

        __MockCharacter->set_busy(1),
        assert(testOb->check_busy(), "regex", "^You are too busy"),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),

        testOb->set_requirements(REQUIREMENT_DISABLE),
        assert(testOb->query_requirements(), "==", REQUIREMENT_DISABLE),
        assert(testOb->check_disable(), "==", 1),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),

        __MockCharacter->set_disable(1),
        assert(testOb->check_disable(), "regex", "^You are not able"),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are not able"),

        // test multiple requirements
        testOb->set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE),
        __MockCharacter->set_busy(0),
        __MockCharacter->set_disable(0),
        assert(testOb->can_verb_rule("verb", "rule"), "==", 1),
        // busy
        __MockCharacter->set_busy(1),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),
        // both
        __MockCharacter->set_disable(1),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are too busy"),
        // disable
        __MockCharacter->set_busy(0),
        assert(testOb->can_verb_rule("verb", "rule"), "regex", "^You are not able"),
    }) :));
    destruct(__MockCharacter);
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
        assert((: testOb->set_help_text(this_object()) :), "catch", "*Bad argument 1 to verb->set_help_text\n"),
        assert((: testOb->set_help_text(1.0) :), "catch", "*Bad argument 1 to verb->set_help_text\n"),
        assert((: testOb->set_help_text(1) :), "catch", "*Bad argument 1 to verb->set_help_text\n"),
        assert((: testOb->set_help_text(({})) :), "catch", "*Bad argument 1 to verb->set_help_text\n"),
        assert((: testOb->set_help_text(([])) :), "catch", "*Bad argument 1 to verb->set_help_text\n"),
        assert((: testOb->set_help_text((: 1 :)) :), "catch", "*Bad argument 1 to verb->set_help_text\n"),
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
        assert((: testOb->set_syntax(this_object()) :), "catch", "*Bad argument 1 to verb->set_syntax\n"),
        assert((: testOb->set_syntax(1.0) :), "catch", "*Bad argument 1 to verb->set_syntax\n"),
        assert((: testOb->set_syntax(1) :), "catch", "*Bad argument 1 to verb->set_syntax\n"),
        assert((: testOb->set_syntax(({})) :), "catch", "*Bad argument 1 to verb->set_syntax\n"),
        assert((: testOb->set_syntax(([])) :), "catch", "*Bad argument 1 to verb->set_syntax\n"),
        assert((: testOb->set_syntax((: 1 :)) :), "catch", "*Bad argument 1 to verb->set_syntax\n"),
    }) :));
}