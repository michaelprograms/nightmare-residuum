#include <driver/function.h>
#include "user.h"

#define STATE_INPUT_NORMAL      0
#define STATE_INPUT_SINGLE      1
#define STATE_INPUT_CHARACTER   2

/*
Data format:
    "inputFn": function
    "prompt": mixed
    "secure": int
    "callbackFn": function
    "lock": int
    "type": int
*/
nosave private mapping *__InputStack = ({ });

private nomask mapping get_top_handler (int require_handler);
private nomask mapping get_bottom_handler ();
nomask void input_pop ();
nomask void input_focus ();

private nomask void dispatch_input (mixed str) {
    mapping input;

    if (str[0] == '!' && !__InputStack[<1]["lock"]) {
        // override ! to bottom handler?
        if (input = get_bottom_handler()) {
            evaluate(input["inputFn"], str[1..]);
        }
    } else {
        if (!(input = get_top_handler(1))) {
            return;
        }
        if (input["type"] == STATE_INPUT_SINGLE) {
            input_pop();
        }
        evaluate(input["inputFn"], str);
    }
    if (this_object()) {
        input_focus();
    }
}

private nomask string process_input (string str) {
    if (!this_object()->query_terminal("type")) {
        this_object()->handle_remove();
    }
    dispatch_input(str);
    return 0;
}

private nomask void stack_push (function inputFn, mixed prompt, int secure, function callbackFn, int lock, int type) {
    mapping input = ([ ]);

    input["inputFn"] = inputFn;
    if (prompt) {
        input["prompt"] = prompt;
    }
    input["secure"] = secure || 0;
    input["callbackFn"] = callbackFn || 0;
    input["lock"] = lock || 0;
    input["type"] = type;
    __InputStack += ({ input });

    if (input["type"] == STATE_INPUT_CHARACTER) {
        efun::get_char((: dispatch_input :), input["secure"] | 2);
    } else {
        efun::input_to((: dispatch_input :), input["secure"] | 2);
    }
}

varargs nomask void input_push (function inputFn, mixed prompt, int secure, function callbackFn, int lock) {
    stack_push(inputFn, prompt, secure, callbackFn, lock, STATE_INPUT_NORMAL);
}
varargs nomask void input_single (function inputFn, mixed prompt, int secure, int lock) {
    stack_push(inputFn, prompt, secure, 0, lock, STATE_INPUT_SINGLE);
}
varargs nomask void input_next (function inputFn, mixed prompt, int secure, int lock) {
    if (!sizeof(__InputStack)) {
        return;
    }
    __InputStack[<1]["inputFn"] = inputFn;
    if (prompt) {
        __InputStack[<1]["prompt"] = prompt;
    }
    __InputStack[<1]["secure"] = secure;
    __InputStack[<1]["lock"] = lock;
}
nomask void input_pop () {
    mapping input;

    __InputStack = __InputStack[0..<2]; // remove last element
    if ((input = get_top_handler(0)) && input["callbackFn"]) {
        evaluate(input["callbackFn"]);
    }
}
nomask varargs void input_prompt (mapping input) {
    string prompt;
    int go_ahead;

    if (!input) {
        if (!(input = get_top_handler(1))) {
            return;
        }
        go_ahead = 1;
    }
    if (input["type"] != STATE_INPUT_CHARACTER && input["prompt"]) {
        if (functionp(input["prompt"])) {
            prompt = evaluate(input["prompt"]);
        } else {
            prompt = input["prompt"];
        }
        if (prompt) {
            message("prompt", prompt, this_object());
        }
        if (go_ahead) {
            telnet_ga();
        }
    }
}
nomask void input_focus () {
    mapping input;

    if (!(input = get_top_handler(1))) {
        return;
    }
    input_prompt(input);
    if (input["type"] == STATE_INPUT_CHARACTER) {
        efun::get_char((: dispatch_input :), input["secure"] | 2);
    } else {
        efun::input_to((: dispatch_input :), input["secure"] | 2);
    }
}

private nomask int create_handler () {
    if (!this_object()) {
        return 0;
    }

    this_object()->shell_start();

    if (!sizeof(__InputStack)) {
        message("system", "Unable to process input.", this_object());
        destruct();
        return 1;
    }
    return 0;
}

private nomask mapping get_top_handler (int require_handler) {
    int some_popped = 0;

    while (sizeof(__InputStack)) {
        mapping input = __InputStack[<1];
        if (!(functionp(input["inputFn"]) & FP_OWNER_DESTED)) {
            if (some_popped && input["callbackFn"]) {
                evaluate(input["callbackFn"]);
            }
            return input;
        }

        __InputStack = __InputStack[0..<2];
        some_popped = 1;
    }

    if (!require_handler || create_handler() || !sizeof(__InputStack)) {
        return 0;
    }
    return __InputStack[<1];
}

private nomask mapping get_bottom_handler () {
    while (sizeof(__InputStack)) {
        mapping input = __InputStack[0];
        if (!(functionp(input["inputFn"]) & FP_OWNER_DESTED)) {
            return input;
        }
        __InputStack = __InputStack[1..];
    }

    if (create_handler()) {
        return 0;
    }
    return __InputStack[0];
}