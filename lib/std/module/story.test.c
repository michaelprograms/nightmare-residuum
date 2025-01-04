inherit M_TEST;

/**
 * @var {"/std/module/story"} testOb
 */

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

private mixed *calloutInfo;
void test_story () {
    expect("stories achieve start, action, final", (: ({
        // no story lines, no story start
        testOb->story_start(0),
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 0),
        testOb->story_start(this_object()),
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 0),

        // no story lines, no story actions
        testOb->story_action(0, 0, 0),
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 0),
        testOb->story_action(this_object(), 0, 0),
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 0),

        // story lines, story starts
        testOb->set_story_lines(({ "1", "2", function(object target) {} })),
        testOb->story_start(this_object()),
        calloutInfo = filter(call_out_info(), (: $1 && $1[0] == testOb :)),
        assert_equal(sizeof(calloutInfo), 1),
        assert_equal(sizeof(calloutInfo[0]), 3),
        assert_equal(calloutInfo[0][0], testOb),

        // force story actions and story final
        testOb->story_action(this_object(), testOb->query_story_lines()[0], testOb->query_story_lines()[1..]),
        testOb->story_action(this_object(), testOb->query_story_lines()[1], testOb->query_story_lines()[2..]),
        testOb->story_action(this_object(), testOb->query_story_lines()[2], testOb->query_story_lines()[3..]),
    }) :));
}