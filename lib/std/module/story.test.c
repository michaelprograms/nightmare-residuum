inherit M_TEST;

void test_lines () {
    expect("lines are settable and queryable", (: ({
        assert_equal(testOb->query_story_lines(), ({ })),

        testOb->set_story_lines(({ "1", "2", "3" })),
        assert_equal(testOb->query_story_lines(), ({ "1", "2", "3" })),

        testOb->set_story_lines(({ (: 1 :), (: 2 :), (: 3 :) })),
        assert_equal(testOb->query_story_lines(), ({ (: 1 :), (: 2 :), (: 3 :) })),
    }) :));
}

void test_delay () {
    expect("delay is settable and queryable", (: ({
        assert_equal(testOb->query_story_delay(), 3),

        testOb->set_story_delay(5),
        assert_equal(testOb->query_story_delay(), 5),

        testOb->set_story_delay(2),
        assert_equal(testOb->query_story_delay(), 2),
    }) :));
}