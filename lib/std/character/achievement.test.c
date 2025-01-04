inherit M_TEST;

/**
 * @var {"/std/character/achievement"} testOb
 */

void test_achievements () {
    expect("null achievements are initialized", (: ({
        assert_equal(testOb->query_achievements_done(), ({ })),
        assert_equal(testOb->query_achievements_incomplete(), ({ })),
        store_variable("__Achievements", UNDEFINED, testOb),
        assert_equal(testOb->query_achievements_done(), ({ })),
        assert_equal(testOb->query_achievements_incomplete(), ({ })),
    }) :));

    expect("achievements are settable and queryable", (: ({
        assert_equal(testOb->query_achievements_done(), ({ })),
        assert_equal(testOb->query_achievements_incomplete(), ({ })),

        assert_equal(testOb->query_achievement_done("test1"), UNDEFINED),
        assert_equal(testOb->query_achievement("test1"), UNDEFINED),
        assert_equal(testOb->set_achievement_flag("test1", "2", "1,2,3"), ([ "done": 0, "flags": "2" ])),
        assert_equal(testOb->query_achievement("test1"), ([ "done": 0, "flags": "2" ])),
        assert_equal(testOb->set_achievement_flag("test1", "2", "1,2,3"), ([ "done": 0, "flags": "2" ])), // already set
        assert_equal(testOb->set_achievement_flag("test1", "3", "1,2,3"), ([ "done": 0, "flags": "2,3" ])),
        assert_equal(testOb->set_achievement_flag("test1", "1", "1,2,3"), ([ "done": 1 ])),
        assert_equal(testOb->query_achievement("test1"), ([ "done": 1 ])),

        assert_equal(testOb->set_achievement_flag("test2", "1", "1"), ([ "done": 1 ])),
        assert_equal(testOb->query_achievement("test2"), ([ "done": 1 ])),
        assert_equal(testOb->query_achievements_done(), ({ "test2", "test1", })),
        testOb->remove_achievement("test2"),
        assert_equal(testOb->query_achievements_done(), ({ "test1", })),
        assert_equal(testOb->query_achievement("test2"), UNDEFINED),
    }) :));
}