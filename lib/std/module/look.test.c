inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/look.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_looks", "test_applies" });
}

void test_looks () {
    mixed *values = ({}), *results = ({});

    expect_function("query_looks", testOb);
    expect_function("query_look", testOb);
    expect_function("set_look", testOb);
    expect_function("set_looks", testOb);
    expect_function("remove_look", testOb);

    values += ({ testOb->query_looks() });
    results += ({ ({}) });
    testOb->set_look("test", "Test.");
    values += ({ testOb->query_looks() });
    results += ({ ({ "test" }) });
    testOb->set_looks(([
        "test": "Test.",
        "quiz": "Quiz.",
        "exam": "Exam.",
    ]));
    values += ({ testOb->query_looks() });
    results += ({ ({ "exam", "quiz", "test" }) });
    values += ({ testOb->query_look("exam") });
    results += ({ "Exam." });
    values += ({ testOb->query_look("test") });
    results += ({ "Test." });
    values += ({ testOb->query_look("quiz") });
    results += ({ "Quiz." });
    testOb->remove_look("quiz");
    values += ({ testOb->query_looks() });
    results += ({ ({ "exam", "test" }) });

    testOb->set_looks(([ "new": "New.", "newFunc": function() {} ]));
    values += ({ testOb->query_looks() });
    results += ({ ({ "new", "newFunc" }) });

    values += ({ testOb->query_look("new") });
    results += ({ "New." });
    values += ({ !!functionp(testOb->query_look("newFunc")) });
    results += ({ 1 });

    testOb->set_looks(([
        ({ "rocks", "stones" }): "Rocks and stones.",
    ]));
    values += ({ testOb->query_looks() });
    results += ({ ({ "rocks", "stones" }) });
    values += ({ testOb->query_look("rock") });
    results += ({ "Rocks and stones." });
    values += ({ testOb->query_look("stone") });
    results += ({ "Rocks and stones." });
    values += ({ testOb->query_look("rocks") });
    results += ({ "Rocks and stones." });
    values += ({ testOb->query_look("stones") });
    results += ({ "Rocks and stones." });

    expect_arrays_equal(values, results, "look handled adding, removing, and querying");
}

nosave private int LookCounter = 0;
void test_applies () {
    mixed *values = ({}), *results = ({});

    expect_function("direct_look_at_str", testOb);
    expect_function("direct_look_str", testOb);
    expect_function("do_look_at_str", testOb);
    expect_function("do_look_str", testOb);

    values += ({ testOb->direct_look_at_str() });
    results += ({ 0 });
    values += ({ testOb->direct_look_str() });
    results += ({ 0 });

    // move_object("/domain/Nowhere/room/void.c");
    // values += ({ testOb->direct_look_at_str() });
    // results += ({ 0 }); // @TODO should be 1 but testOb needs handle_move
    // values += ({ testOb->direct_look_str() });
    // results += ({ 0 }); // @TODO should be 1 but testOb needs handle_move

    testOb->set_look("test", function(object character) {
        LookCounter ++;
        return; // @TODO check returning something and receive_message
    });
    values += ({ testOb->query_looks() });
    results += ({ ({ "test" }) });
    testOb->do_look_at_str("test");
    values += ({ LookCounter });
    results += ({ 1 });
    testOb->do_look_str("test");
    values += ({ LookCounter });
    results += ({ 2 });

    expect_arrays_equal(values, results, "look applies handled");
}

// @TODO bad arguments