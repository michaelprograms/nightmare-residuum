inherit M_TEST;
inherit M_MOVE;

/**
 * @var {"/std/module/look"} testOb
 */

string *test_order () {
    return ({ "test_looks", "test_handle_look" });
}

void test_looks () {
    expect("null looks are initialized", (: ({
        assert_equal(testOb->query_looks(), ({ })),
        store_variable("__Looks", UNDEFINED, testOb),
        assert_equal(testOb->query_looks(), ({ })),
    }) :));

    expect("look handles setting, querying, and removing", (: ({
        assert_equal(testOb->query_looks(), ({ })),
        testOb->set_look("test", "Test."),
        assert_equal(testOb->query_looks(), ({ "test" })),

        testOb->set_looks(([
            "test": "Test.",
            "quiz": "Quiz.",
            "exam": "Exam.",
        ])),
        assert_equal(testOb->query_looks(), ({ "exam", "quiz", "test" })),
        assert_equal(testOb->query_look("exam"), "Exam."),
        assert_equal(testOb->query_look("test"), "Test."),
        assert_equal(testOb->query_look("quiz"), "Quiz."),
        assert_equal(testOb->query_look(UNDEFINED), 0),

        testOb->remove_look("quiz"),
        assert_equal(testOb->query_looks(), ({ "exam", "test" })),

        testOb->set_looks(([ "new": "New.", "newFunc": function() {} ])),
        assert_equal(testOb->query_looks(), ({ "new", "newFunc" })),

        assert_equal(testOb->query_look("new"), "New."),
        assert_equal(!!functionp(testOb->query_look("newFunc")), 1),

        testOb->set_looks(([
            ({ "rocks", "stones" }): "Rocks and stones.",
        ])),
        assert_equal(testOb->query_looks(), ({ "rocks", "stones" })),
        assert_equal(testOb->query_look("rock"), "Rocks and stones."),
        assert_equal(testOb->query_look("stone"), "Rocks and stones."),
        assert_equal(testOb->query_look("rocks"), "Rocks and stones."),
        assert_equal(testOb->query_look("stones"), "Rocks and stones."),

        assert_catch((: testOb->set_look(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to look->set_look\n"),
        assert_catch((: testOb->set_look("bad", UNDEFINED) :), "*Bad argument 2 to look->set_look\n"),
        assert_catch((: testOb->set_looks(UNDEFINED) :), "*Bad argument 1 to look->set_looks\n"),
        assert_catch((: testOb->remove_look(UNDEFINED) :), "*Bad argument 1 to look->remove_look\n"),
    }) :));
}

nosave private int LookCounter = 0;
void test_handle_look () {
    expect("handle_look returns description", (: ({
        testOb->set_look("test", function (object character) {
            LookCounter ++;
            return "Test description. " + LookCounter;
        }),
        testOb->set_look("quiz", "Quiz description."),
        assert_equal(testOb->query_looks(), ({ "quiz", "test", })),
        assert_equal(testOb->handle_look("test"), "Test description. 1"),
        assert_equal(LookCounter, 1),
        assert_equal(testOb->handle_look("test"), "Test description. 2"),
        assert_equal(LookCounter, 2),
        assert_equal(testOb->handle_look("quiz"), "Quiz description."),

        assert_equal(testOb->handle_look("invalid"), 0),

        assert_catch((: testOb->handle_look(UNDEFINED) :), "*Bad argument 1 to look->handle_look\n"),
    }) :));
}