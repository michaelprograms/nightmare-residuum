inherit M_TEST;

/**
 * @var {"/secure/std/pager"} testOb
 */

void test_pager () {
    // 41 lines exceeds the default chunk size (40), so the pager waits for
    // input rather than auto-finishing and destructing testOb mid-test
    string *lines = map(allocate(41), (: "x" :));
    expect("pager sets user on start", (: ({
        testOb->start($(lines), this_object()),
        assert_equal(testOb->query_user(), this_object()),
    }) :));
}
