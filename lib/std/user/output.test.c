inherit M_TEST;

/**
 * @var {"/std/user/output"} testOb
 */

/** @type {"/std/user/output.mock"} */
nosave private object __Mock;

void before_each_test() {
    __Mock = new("/std/user/output.mock.c");
    __Mock->start_shadow(testOb);
}
void after_each_test() {
    if (__Mock) {
        __Mock->stop_shadow();
        destruct(__Mock);
    }
}

void test_handle_pager() {
    expect("handle_pager pages a string or an array of lines", (: ({
        // the prompt shows how many lines the pager received
        testOb->handle_pager("a\nb"),
        assert_regex(evaluate(__Mock->query_prompt()), "Lines 1 to 1 of 2 "),
        evaluate(__Mock->query_page(), -1),

        testOb->handle_pager(({ "c", "d", "e" })),
        assert_regex(evaluate(__Mock->query_prompt()), "Lines 1 to 1 of 3 "),
        evaluate(__Mock->query_page(), -1),
    }) :));
}
