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
    }) :));
}

nosave private int LookCounter = 0;
void test_handle_look () {
    expect_function("handle_look", testOb);

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
    }) :));
}