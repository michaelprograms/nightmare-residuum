inherit M_TEST;

/**
 * @var {"/secure/sefun/test"} testOb
 */

void test_format_string_difference () {
    expect("format_string_difference labels two lines", (: ({
        assert_equal(testOb->format_string_difference("staffs", "staves"), "    - '\e[42msta\e[0m\e[43mves\e[0m'\n    + '\e[42msta\e[0m\e[41mffs\e[0m'"),
        assert_equal(testOb->format_string_difference("foo", "bar"), "    - '\e[43mbar\e[0m'\n    + '\e[41mfoo\e[0m'"),
        assert_equal(testOb->format_string_difference("foo", "foo"), "    - '\e[42mfoo\e[0m\e[43m\e[0m'\n    + '\e[42mfoo\e[0m\e[41m\e[0m'"),
        assert_equal(testOb->format_string_difference("", ""), "    - '\e[43m\e[0m'\n    + '\e[41m\e[0m'"),

        assert_equal(testOb->format_string_difference(([ "a": 1, "b": 2, "c": 3, ]), ([ "a": 1, "b": 2, "c": 3, ])), "    - '\e[42m([ \"a\": 1, \"b\": 2, \"c\": 3 ])\e[0m\e[43m\e[0m'\n    + '\e[42m([ \"a\": 1, \"b\": 2, \"c\": 3 ])\e[0m\e[41m\e[0m'"),
    }) :));
    expect("format_string_difference escapes special characters", (: ({
        assert_equal(testOb->format_string_difference("foo\nbar", "foo\nbaz"), "    - '\e[42mfoo\\nba\e[0m\e[43mz\e[0m'\n    + '\e[42mfoo\\nba\e[0m\e[41mr\e[0m'"),
    }) :));
    expect("format_string_difference handles non-string inputs", (: ({
        assert_equal(testOb->format_string_difference("42", "43"), "    - '\e[42m4\e[0m\e[43m3\e[0m'\n    + '\e[42m4\e[0m\e[41m2\e[0m'"),
    }) :));
}

void test_format_array_differences () {
    expect("format_array_differences labels two lines per entry", (: ({
        assert_equal(testOb->format_array_differences(({ "staffs" }), ({ "staves" })), "\n       0. - '\e[42msta\e[0m\e[43mves\e[0m'\n          + '\e[42msta\e[0m\e[41mffs\e[0m'"),
        assert_equal(testOb->format_array_differences(({ "foo" }), ({ "foo" })), "\n       0. - '\e[42mfoo\e[0m\e[43m\e[0m'\n          + '\e[42mfoo\e[0m\e[41m\e[0m'"),
    }) :));
    expect("format_array_differences handles mismatched lengths", (: ({
        assert_equal(testOb->format_array_differences(({ "foo", "bar" }), ({ "foo" })), "\n       0. - '[42mfoo[0m[43m[0m'\n          + '[42mfoo[0m[41m[0m'\n       1. - '[43m[0m'\n          + '[41mbar[0m'"),
        assert_equal(testOb->format_array_differences(({ "foo" }), ({ "foo", "bar" })), "\n       0. - '[42mfoo[0m[43m[0m'\n          + '[42mfoo[0m[41m[0m'\n       1. - '[43mbar[0m'\n          + '[41m[0m'"),
    }) :));
}
