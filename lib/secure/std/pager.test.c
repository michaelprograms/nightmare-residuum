inherit M_TEST;

/**
 * @var {"/secure/std/pager"} testOb
 */

nosave private int __Lines, __Pops, __Focuses, __MessageError;
nosave private function __Page, __Prompt;
nosave private string *__Messages;

void before_each_test() {
    __Lines = 0;
    __Pops = 0;
    __Focuses = 0;
    __MessageError = 0;
    __Page = 0;
    __Prompt = 0;
    __Messages = ({});
}

/* ----- stands in for the user, see /std/user.c ----- */

mixed query_setting(string key) {
    return __Lines;
}
varargs void input_push(function inputFn, mixed prompt) {
    __Page = inputFn;
    __Prompt = prompt;
}
void input_pop() {
    __Pops++;
}
void input_focus() {
    __Focuses++;
}
void receive_message(string type, string message) {
    if (__MessageError) {
        error("receive_message error");
    }
    __Messages += ({ message });
}

/* ----- tests ----- */

void test_pager() {
    // 41 lines exceeds the default chunk size (40), so the pager waits for
    // input rather than auto-finishing and destructing testOb mid-test
    string *lines = map(allocate(41), "x");
    expect("pager sets user on start", (: ({
        testOb->start($(lines), this_object()),
        assert_equal(testOb->query_user(), this_object()),
    }) :));
}

void test_start_without_user() {
    expect("pager ignores start without a user", (: ({
        testOb->start(({ "a" }), UNDEFINED),
        assert_equal(testOb->query_user(), UNDEFINED),
        assert_equal(__Messages, ({})),
    }) :));
}

void test_short_output() {
    expect("pager outputs and finishes when lines fit on one page", (: ({
        testOb->start(({ "a", "b" }), this_object()),
        assert_equal(__Messages, ({ "a\n", "b\n" })),
        assert_equal(__Pops, 1),
        assert_equal(__Focuses, 0),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_paging() {
    __Lines = 2;

    expect("pager pages through lines", (: ({
        testOb->start(({ "1", "2", "3", "4", "5" }), this_object()),
        assert_equal(__Messages, ({ "1\n", "2\n" })),
        assert_regex(evaluate(__Prompt), "Lines 1 to 2 of 5 \\(40%\\)"),

        evaluate(__Page, ""),
        assert_equal(__Messages, ({ "1\n", "2\n", "3\n", "4\n" })),
        assert_regex(evaluate(__Prompt), "Lines 3 to 4 of 5 \\(80%\\)"),

        evaluate(__Page, ""),
        assert_equal(__Messages, ({ "1\n", "2\n", "3\n", "4\n", "5\n" })),
        assert_equal(evaluate(__Prompt), ""),
        assert_equal(__Pops, 1),
        assert_equal(__Focuses, 1),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_quit() {
    __Lines = 2;

    expect("pager quits on q", (: ({
        testOb->start(({ "1", "2", "3" }), this_object()),
        evaluate(__Page, "q"),
        assert_equal(__Messages, ({ "1\n", "2\n" })),
        assert_equal(__Pops, 1),
        assert_equal(__Focuses, 0),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_abort() {
    __Lines = 2;

    expect("pager destructs on -1", (: ({
        testOb->start(({ "1", "2", "3" }), this_object()),
        evaluate(__Page, -1),
        assert_equal(__Pops, 0),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_page_error() {
    __MessageError = 1;

    expect("pager restores input when the first page errors", (: ({
        testOb->start(({ "a" }), this_object()),
        assert_equal(__Pops, 1),
        assert_equal(__Focuses, 1),
        assert_equal(objectp(testOb), 1),
    }) :));
}
