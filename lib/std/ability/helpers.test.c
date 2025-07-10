inherit M_TEST;

/**
 * @var {"/std/ability"} testOb
 */

nosave private string __MockClass;
string query_class () { return __MockClass; }

void test_die_sides () {
    expect("handles die sides", (: ({
        // default
        assert_equal(testOb->query_die_sides(this_object()), 6),

        // psionist
        __MockClass = "psionist",
        assert_equal(testOb->query_die_sides(this_object()), 8),

        __MockClass = "mystic",
        assert_equal(testOb->query_die_sides(this_object()), 7),
    }) :));
}