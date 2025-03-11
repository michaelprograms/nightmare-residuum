inherit M_TEST;

/**
 * @var {"/secure/sefun/banner"} testOb
 */

void test_banner () {
    string banner = testOb->query_banner();
    expect("handles banner", (: ({
        assert_equal(sizeof($(banner)) > 0, 1),
        assert_equal(sizeof(explode($(banner), "\n")), 9),
    }) :));
}