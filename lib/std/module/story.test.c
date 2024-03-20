inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/story.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_lines () {
    expect_function("query_story_lines", testOb);
    expect_function("set_story_lines", testOb);

    expect("lines are settable and queryable", (: ({
        assert_equal(testOb->query_story_lines(), ({ })),

        testOb->set_story_lines(({ "1", "2", "3" })),
        assert_equal(testOb->query_story_lines(), ({ "1", "2", "3" })),

        testOb->set_story_lines(({ (: 1 :), (: 2 :), (: 3 :) })),
        assert_equal(testOb->query_story_lines(), ({ (: 1 :), (: 2 :), (: 3 :) })),
    }) :));
}

void test_delay () {
    expect_function("query_story_delay", testOb);
    expect_function("set_story_delay", testOb);

    expect("delay is settable and queryable", (: ({
        assert_equal(testOb->query_story_delay(), 3),

        testOb->set_story_delay(5),
        assert_equal(testOb->query_story_delay(), 5),

        testOb->set_story_delay(2),
        assert_equal(testOb->query_story_delay(), 2),
    }) :));
}