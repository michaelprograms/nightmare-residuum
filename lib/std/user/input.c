#include <driver/function.h>
#include "user.h"

#define STATE_INPUT_NORMAL      0
#define STATE_INPUT_SINGLE      1
#define STATE_INPUT_CHARACTER   2

inherit "/std/class/user_input.c";

nosave private class Input *stack = ({ });

private nomask class Input get_top_handler (int require_handler);
private nomask class Input get_bottom_handler ();

nomask int query_input_stack_size () {
    return sizeof(stack);
}

private nomask void dispatch_to_bottom (mixed str) {
    class Input info;
    if (!(info = get_bottom_handler())) return;
    evaluate(info->inputFn, str);
}
private nomask void dispatch_input (mixed str) {
    class Input info;
    if (str[0] == '!' && !stack[<1]->lock) {
        dispatch_to_bottom(str[1..]); // override ! to shell
    } else {
        if (!(info = get_top_handler(1))) return;
        if (info->type == STATE_INPUT_SINGLE) input_pop();
        evaluate(info->inputFn, str);
    }
    if (this_object()) input_focus();
}

private nomask string process_input (string str) {
    if (!this_object()->query_terminal_type()) {
        this_object()->handle_remove();
    }
    dispatch_input(str);
    return 0;
}

private nomask void stack_push (function inputFn, mixed prompt, int secure, function callbackFn, int lock, int type) {
    class Input info = new(class Input);
    info->inputFn = inputFn;
    if (prompt) info->prompt = prompt;
    info->secure = secure;
    info->callbackFn = callbackFn;
    info->lock = lock;
    info->type = type;
    stack += ({ info });
    if (info->type == STATE_INPUT_CHARACTER) {
        efun::get_char((: dispatch_input :), info->secure | 2);
    } else {
        efun::input_to((: dispatch_input :), info->secure | 2);
    }
}

varargs nomask void input_push (function inputFn, mixed prompt, int secure, function callbackFn, int lock) {
    stack_push(inputFn, prompt, secure, callbackFn, lock, STATE_INPUT_NORMAL);
}
varargs nomask void input_single (function inputFn, mixed prompt, int secure, int lock) {
    stack_push(inputFn, prompt, secure, 0, lock, STATE_INPUT_SINGLE);
}
varargs nomask void input_next (function inputFn, mixed prompt, int secure, int lock) {
    stack[<1]->inputFn = inputFn;
    if (prompt) stack[<1]->prompt = prompt;
    stack[<1]->secure = secure;
    stack[<1]->lock = lock;
}
nomask void input_pop () {
    class Input info;

    stack = stack[0..<2]; // remove last element

    if ((info = get_top_handler(0)) && info->callbackFn) {
        evaluate(info->callbackFn);
    }
}
nomask void input_focus () {
    class Input info;
    string prompt;

    if (!(info = get_top_handler(1))) return;
    if (info->type != STATE_INPUT_CHARACTER && info->prompt) {
        if (functionp(info->prompt)) prompt = evaluate(info->prompt);
        else prompt = info->prompt;
        if (prompt) message("prompt", prompt, this_object());
    }
    if (info->type == STATE_INPUT_CHARACTER) {
        efun::get_char((: dispatch_input :), info->secure | 2);
    } else {
        efun::input_to((: dispatch_input :), info->secure | 2);
    }
}

private nomask int create_handler () {
    if (!this_object()) return 0;

    this_object()->shell_start();

    if (!sizeof(stack)) {
        message("system", "Unable to process input.", this_object());
        destruct();
        return 1;
    }
    return 0;
}

private nomask class Input get_top_handler (int require_handler) {
    int some_popped = 0;

    while (sizeof(stack)) {
        class Input info;

        info = stack[<1];
        if (!(functionp(info->inputFn) & FP_OWNER_DESTED)) {
            if (some_popped && info->callbackFn)
            evaluate(info->callbackFn);
            return info;
        }

        stack = stack[0..<2];
        some_popped = 1;
    }

    if (!require_handler || create_handler() || !sizeof(stack)) return 0;
    return stack[<1];
}

private nomask class Input get_bottom_handler () {
    while (sizeof(stack)) {
        class Input info;
        info = stack[0];
        if (!(functionp(info->inputFn) & FP_OWNER_DESTED)) return info;
        stack = stack[1..];
    }

    if (create_handler()) return 0;
    return stack[0];
}