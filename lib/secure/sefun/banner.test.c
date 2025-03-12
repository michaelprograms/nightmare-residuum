inherit M_TEST;

/**
 * @var {"/secure/sefun/banner"} testOb
 */

string __TermColor;
string query_terminal (string type) {
    if (type == "color") {
        return __TermColor;
    }
}

void test_banner () {
    string banner;

    banner = testOb->query_banner();
    expect("handles banner 16 color mode", (: ({
        assert_equal(sizeof($(banner)) > 0, 1),
        assert_equal(sizeof(explode($(banner), "\n")), 9),
    }) :));

    __TermColor = "256";
    banner = testOb->query_banner();
    expect("handles banner 256 color mode", (: ({
        assert_equal(sizeof($(banner)) > 0, 1),
        assert_equal(sizeof(explode($(banner), "\n")), 9),
    }) :));
}