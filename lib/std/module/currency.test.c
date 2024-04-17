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

void test_currencies () {
    expect("query_currency returns -1 for invalid currency", (: ({
        assert_equal(testOb->query_currency("unknown"), -1),
        assert_equal(testOb->query_currency("invalid"), -1),
    }) :));
    expect("currency handles adding and querying", (: ({
        assert_equal(testOb->query_currencies(), ({ })),
        assert_equal(testOb->query_currency("copper"), 0),

        assert_equal(testOb->add_currency("copper", -10), 0), // can't remove
        assert_equal(testOb->query_currencies(), ({ })),
        assert_equal(testOb->query_currency("copper"), 0), // still 0

        assert_equal(testOb->add_currency("copper", 10), 1), // add
        assert_equal(testOb->query_currencies(), ({ "copper" })),
        assert_equal(testOb->query_currency("copper"), 10),

        assert_equal(testOb->add_currency("copper", -100), 0), // can't remove
        assert_equal(testOb->query_currencies(), ({ "copper" })),
        assert_equal(testOb->query_currency("copper"), 10), // still 10

        assert_equal(testOb->add_currency("copper", -5), 1), // can remove
        assert_equal(testOb->query_currencies(), ({ "copper" })), // some remains
        assert_equal(testOb->query_currency("copper"), 5),
    }) :));
}