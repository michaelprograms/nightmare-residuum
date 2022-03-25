inherit M_TEST;
inherit STD_LIVING;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/look.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_looks", "test_applies" });
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
}

nosave private int LookCounter = 0;
void test_applies () {
    expect_function("direct_look_at_str", testOb);
    expect_function("direct_look_str", testOb);
    expect_function("do_look_at_str", testOb);
    expect_function("do_look_str", testOb);

    // setup test object
    // if (testOb) destruct(testOb);
    // testOb = new(STD_LIVING); // need living for handle_move

    expect("look handles applies", (: ({
        assert(testOb->direct_look_at_str(), "==", 0),
        assert(testOb->direct_look_str(), "==", 0),

        // @TODO how to test module/look when it doesn't have handle_move
        // assert(testOb->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        // assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
        // assert(testOb->direct_look_at_str(), "==", 1),
        // assert(testOb->direct_look_str(), "==", 1),

        testOb->set_look("test", function(object character) {
            LookCounter ++;
            return; // @TODO check returning something and receive_message
        }),
        assert(testOb->query_looks(), "==", ({ "test" })),
        testOb->do_look_at_str("test"),
        assert(LookCounter, "==", 1),
        testOb->do_look_str("test"),
        assert(LookCounter, "==", 2),
    }) :));
}

// @TODO bad arguments