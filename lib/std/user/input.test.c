#include <driver/function.h>

inherit M_TEST;

/**
 * @var {"/std/user/input"} testOb
 */

nosave private int __Prompts, __Callbacks;

void before_each_test() {
    __Prompts = 0;
    __Callbacks = 0;
}

void handle_input(string _str) {}
string query_prompt() {
    __Prompts++;
    return "test> ";
}
string query_no_prompt() {
    __Prompts++;
    return 0;
}
void handle_callback() {
    __Callbacks++;
}

void test_input_prompt() {
    function input = (: handle_input :), prompt = (: query_prompt :);

    expect("input_prompt evaluates the top handler's prompt", (: ({
        testOb->input_push($(input), $(prompt)),
        assert_equal(__Prompts, 0),

        testOb->input_prompt(),
        assert_equal(__Prompts, 1),

        testOb->input_focus(),
        assert_equal(__Prompts, 2),

        // a given handler skips the lookup
        testOb->input_prompt(([ "prompt": $(prompt) ])),
        assert_equal(__Prompts, 3),
    }) :));
}

void test_input_prompt_types() {
    function input = (: handle_input :), noPrompt = (: query_no_prompt :);

    expect("input_prompt handles string, empty, and missing prompts", (: ({
        testOb->input_push($(input), "string> "),
        testOb->input_prompt(),
        assert_equal(__Prompts, 0),

        // prompt function returning nothing
        testOb->input_push($(input), $(noPrompt)),
        testOb->input_prompt(),
        assert_equal(__Prompts, 1),

        // no prompt at all
        testOb->input_push($(input)),
        testOb->input_prompt(),
        assert_equal(__Prompts, 1),
    }) :));
}

void test_input_next() {
    function input = (: handle_input :), prompt = (: query_prompt :);

    expect("input_next replaces the top handler", (: ({
        // nothing to replace
        testOb->input_next($(input), $(prompt)),

        testOb->input_push($(input), "string> "),
        testOb->input_next($(input), $(prompt), 1, 1),
        testOb->input_prompt(),
        assert_equal(__Prompts, 1),

        // no prompt keeps the current prompt
        testOb->input_next($(input)),
        testOb->input_prompt(),
        assert_equal(__Prompts, 2),
    }) :));
}

void test_input_pop() {
    function input = (: handle_input :), prompt = (: query_prompt :);
    function callback = (: handle_callback :);

    expect("input_pop returns to the previous handler", (: ({
        testOb->input_push($(input), $(prompt), 0, $(callback)),
        testOb->input_push($(input), "string> "),

        testOb->input_pop(),
        assert_equal(__Callbacks, 1),
        testOb->input_prompt(),
        assert_equal(__Prompts, 1),

        // emptying the stack doesn't need a new handler
        testOb->input_pop(),
        assert_equal(__Callbacks, 1),
        assert_equal(objectp(testOb), 1),
    }) :));
}

void test_destructed_handlers() {
    function input = (: handle_input :), prompt = (: query_prompt :);
    function callback = (: handle_callback :), mockInput;
    object mock = new("/std/user/input.mock.c");

    mockInput = mock->query_input_fn();
    testOb->input_push(input, prompt, 0, callback);
    testOb->input_push(mockInput);
    destruct(mock);

    expect("handlers of destructed objects are skipped", (: ({
        assert_equal(
            functionp($(mockInput)) & FP_OWNER_DESTED,
            FP_OWNER_DESTED
        ),

        testOb->input_prompt(),
        assert_equal(__Callbacks, 1),
        assert_equal(__Prompts, 1),
    }) :));
}

void test_shell_start() {
    /** @type {"/std/user/input.mock"} */
    object mock = new("/std/user/input.mock.c");

    mock->start_shadow(testOb);

    expect("an empty stack asks shell_start for a handler", (: ({
        assert_equal($(mock)->query_prompts(), 0),
        testOb->input_prompt(),
        assert_equal($(mock)->query_prompts(), 1),
        assert_equal(objectp(testOb), 1),
    }) :));

    mock->stop_shadow();
    destruct(mock);
}

void test_no_handler_prompt() {
    expect("input_prompt without any handler removes the user", (: ({
        testOb->input_prompt(),
        assert_equal(objectp(testOb), 0),
    }) :));
}

void test_no_handler_focus() {
    expect("input_focus without any handler removes the user", (: ({
        testOb->input_focus(),
        assert_equal(objectp(testOb), 0),
    }) :));
}
