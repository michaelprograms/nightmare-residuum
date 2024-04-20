inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

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