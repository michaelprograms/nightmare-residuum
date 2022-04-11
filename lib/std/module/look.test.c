inherit M_TEST;
inherit M_MOVE;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/look.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_looks", "test_handle_look" });
}

void test_looks () {
    expect_function("query_looks", testOb);
    expect_function("query_look", testOb);
    expect_function("set_look", testOb);
    expect_function("set_looks", testOb);
    expect_function("remove_look", testOb);

    expect("look handles setting, querying, and removing", (: ({
        assert(testOb->query_looks(), "==", ({})),
        testOb->set_look("test", "Test."),
        assert(testOb->query_looks(), "==", ({ "test" })),

        testOb->set_looks(([
            "test": "Test.",
            "quiz": "Quiz.",
            "exam": "Exam.",
        ])),
        assert(testOb->query_looks(), "==", ({ "exam", "quiz", "test" })),
        assert(testOb->query_look("exam"), "==", "Exam."),
        assert(testOb->query_look("test"), "==", "Test."),
        assert(testOb->query_look("quiz"), "==", "Quiz."),

        testOb->remove_look("quiz"),
        assert(testOb->query_looks(), "==", ({ "exam", "test" })),

        testOb->set_looks(([ "new": "New.", "newFunc": function() {} ])),
        assert(testOb->query_looks(), "==", ({ "new", "newFunc" })),

        assert(testOb->query_look("new"), "==", "New."),
        assert(!!functionp(testOb->query_look("newFunc")), "==", 1),

        testOb->set_looks(([
            ({ "rocks", "stones" }): "Rocks and stones.",
        ])),
        assert(testOb->query_looks(), "==", ({ "rocks", "stones" })),
        assert(testOb->query_look("rock"), "==", "Rocks and stones."),
        assert(testOb->query_look("stone"), "==", "Rocks and stones."),
        assert(testOb->query_look("rocks"), "==", "Rocks and stones."),
        assert(testOb->query_look("stones"), "==", "Rocks and stones."),
    }) :));

    expect("set_look handles bad argument 1", (: ({
        assert((: testOb->set_look() :), "catch", "*Bad argument 1 to look->set_look\n"),
        assert((: testOb->set_look(0) :), "catch", "*Bad argument 1 to look->set_look\n"),
        assert((: testOb->set_look(0.0) :), "catch", "*Bad argument 1 to look->set_look\n"),
        assert((: testOb->set_look(({})) :), "catch", "*Bad argument 1 to look->set_look\n"),
        assert((: testOb->set_look(([])) :), "catch", "*Bad argument 1 to look->set_look\n"),
        assert((: testOb->set_look((: 1 :)) :), "catch", "*Bad argument 1 to look->set_look\n"),
    }) :));
    expect("set_look handles bad argument 2", (: ({
        assert((: testOb->set_look("") :), "catch", "*Bad argument 2 to look->set_look\n"),
        assert((: testOb->set_look("", 0) :), "catch", "*Bad argument 2 to look->set_look\n"),
        assert((: testOb->set_look("", 0.0) :), "catch", "*Bad argument 2 to look->set_look\n"),
        assert((: testOb->set_look("", ({})) :), "catch", "*Bad argument 2 to look->set_look\n"),
        assert((: testOb->set_look("", ([])) :), "catch", "*Bad argument 2 to look->set_look\n"),
    }) :));

    expect("set_looks handles bad argument 1", (: ({
        assert((: testOb->set_looks() :), "catch", "*Bad argument 1 to look->set_looks\n"),
        assert((: testOb->set_looks("") :), "catch", "*Bad argument 1 to look->set_looks\n"),
        assert((: testOb->set_looks(0) :), "catch", "*Bad argument 1 to look->set_looks\n"),
        assert((: testOb->set_looks(0.0) :), "catch", "*Bad argument 1 to look->set_looks\n"),
        assert((: testOb->set_looks(({})) :), "catch", "*Bad argument 1 to look->set_looks\n"),
        assert((: testOb->set_looks((: 1 :)) :), "catch", "*Bad argument 1 to look->set_looks\n"),
    }) :));

    expect("remove_look handles bad argument 1", (: ({
        assert((: testOb->remove_look() :), "catch", "*Bad argument 1 to look->remove_look\n"),
        assert((: testOb->remove_look(0) :), "catch", "*Bad argument 1 to look->remove_look\n"),
        assert((: testOb->remove_look(0.0) :), "catch", "*Bad argument 1 to look->remove_look\n"),
        assert((: testOb->remove_look(({})) :), "catch", "*Bad argument 1 to look->remove_look\n"),
        assert((: testOb->remove_look(([])) :), "catch", "*Bad argument 1 to look->remove_look\n"),
        assert((: testOb->remove_look((: 1 :)) :), "catch", "*Bad argument 1 to look->remove_look\n"),
    }) :));
}

nosave private int LookCounter = 0;
void test_handle_look () {
    expect_function("handle_look", testOb);

    expect("handle_look returns description", (: ({
        testOb->set_look("test", function(object character) {
            LookCounter ++;
            return "Test description. " + LookCounter;
        }),
        testOb->set_look("quiz", "Quiz description."),
        assert(testOb->query_looks(), "==", ({ "quiz", "test", })),
        assert(testOb->handle_look("test"), "==", "Test description. 1"),
        assert(LookCounter, "==", 1),
        assert(testOb->handle_look("test"), "==", "Test description. 2"),
        assert(LookCounter, "==", 2),
        assert(testOb->handle_look("quiz"), "==", "Quiz description."),
    }) :));

    expect("handle_look handles bad argument 1", (: ({
        assert((: testOb->handle_look() :), "catch", "*Bad argument 1 to look->handle_look\n"),
        assert((: testOb->handle_look(0) :), "catch", "*Bad argument 1 to look->handle_look\n"),
        assert((: testOb->handle_look(0.0) :), "catch", "*Bad argument 1 to look->handle_look\n"),
        assert((: testOb->handle_look(({})) :), "catch", "*Bad argument 1 to look->handle_look\n"),
        assert((: testOb->handle_look(([])) :), "catch", "*Bad argument 1 to look->handle_look\n"),
        assert((: testOb->handle_look((: 1 :)) :), "catch", "*Bad argument 1 to look->handle_look\n"),
    }) :));
}