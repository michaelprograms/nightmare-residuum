inherit M_TEST;

void test_valid_name () {
    expect("query_valid_name handles names", (: ({
        assert_equal(testOb->query_valid_name("valid"), 1),
        assert_equal(testOb->query_valid_name("abcdefghijklm"), 1),
        assert_equal(testOb->query_valid_name("nopqrstuvwxyz"), 1),
        assert_equal(testOb->query_valid_name("a'b c-d"), 1), // valid decorators

        assert_equal(testOb->query_valid_name(""), 0),
        assert_equal(testOb->query_valid_name("abc"), 0), // too short
        assert_equal(testOb->query_valid_name("abcdefghijklmnopqrstuvwxyz"), 0), // too long
        assert_equal(testOb->query_valid_name("!@#$%^&*()"), 0), // invalid chars
    }) :));
}

void test_save_path () {
    expect("query_save_path handles valid names", (: ({
        assert_regex(testOb->query_save_path("name", "type"), "^/save/character/n/name/type"),
    }) :));
}