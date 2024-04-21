inherit M_TEST;

void test_achievements () {
    expect("achievements are settable and queryable", (: ({
        assert_equal(testOb->query_achievements(), ({ })),

        assert_equal(testOb->set_achievement("test1"), 1),
        assert_equal(testOb->query_achievement("test1"), 1),
        assert_equal(testOb->set_achievement("test1"), 0), // already set
        assert_equal(testOb->query_achievement("test1"), 1),

        assert_equal(testOb->set_achievement("test2"), 1),
        assert_equal(testOb->query_achievement("test2"), 1),
        assert_equal(testOb->set_achievement("test2"), 0), // already set
        assert_equal(testOb->query_achievement("test2"), 1),

        assert_equal(testOb->query_achievements(), ({ "test2", "test1", })),

        testOb->remove_achievement("test2"),
        assert_equal(testOb->query_achievements(), ({ "test1", })),
    }) :));
}