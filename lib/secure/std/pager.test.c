inherit M_TEST;

/**
 * @var {"/secure/std/pager"} testOb
 */

void test_pager () {
    expect("pager starts", (: ({
        testOb->start(({ "1", "2", "3", }), this_object()),
        assert_equal(testOb->query_user(), this_object()),
    }) :));
}