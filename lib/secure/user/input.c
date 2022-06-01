#include <driver/function.h>
#include "user.h"

#define STATE_INPUT_NORMAL      0
#define STATE_INPUT_SINGLE      1
#define STATE_INPUT_CHARACTER   2

inherit "/std/class/user_input.c";

nosave private struct Input *stack = ({ });
nosave private int dispatchTarget;

private struct Input get_top_handler (int require_handler);
private struct Input get_bottom_handler ();

int query_input_stack_size () {
    return sizeof(stack);
}

private void dispatch_to_bottom (mixed str) {
    struct Input info;
    if (!(info = get_bottom_handler())) return;
    dispatchTarget = 0;
    funcall(info->inputFn, str);
}
private void dispatch_input (mixed str) {
    struct Input info;
    if (str[0] == '!' && !stack[<1]->lock) {
        dispatch_to_bottom(str[1..]); // override ! to shell
    } else {
        if (!(info = get_top_handler(1))) return;
        if (info->type == STATE_INPUT_SINGLE) input_pop();
        dispatchTarget = sizeof(stack);
        funcall(info->inputFn, str);
    }
    if (this_object()) input_focus();
}

private void process_input (string str) {
    dispatch_input(str);
}

private void stack_push (closure inputFn, mixed prompt, int secure, closure callbackFn, int lock, int type) {
    struct Input info = (<Input>);
    info->inputFn = inputFn;
    if (prompt) info->prompt = prompt;
    info->secure = secure;
    info->callbackFn = callbackFn;
    info->lock = lock;
    info->type = type;
    stack += ({ info });
    // if (info->type == STATE_INPUT_CHARACTER) {
    //     efun::get_char(#'dispatch_input, info->secure | 2);
    // } else {
        efun::input_to(#'dispatch_input, info->secure | 2); // '
    // }
}

varargs void input_push (closure inputFn, mixed prompt, int secure, closure callbackFn, int lock) {
    stack_push(inputFn, prompt, secure, callbackFn, lock, STATE_INPUT_NORMAL);
}
varargs void input_single (closure inputFn, mixed prompt, int secure, int lock) {
    stack_push(inputFn, prompt, secure, 0, lock, STATE_INPUT_SINGLE);
}
varargs void input_next (closure inputFn, mixed prompt, int secure, int lock) {
    stack[<1]->inputFn = inputFn;
    if (prompt) stack[<1]->prompt = prompt;
    stack[<1]->secure = secure;
    stack[<1]->lock = lock;
}
void input_pop () {
    struct Input info;

    stack = stack[0..<2]; // remove last element

    if ((info = get_top_handler(0)) && info->callbackFn) {
        funcall(info->callbackFn);
    }
}
void input_focus () {
    struct Input info;
    string prompt;

    if (!(info = get_top_handler(1))) return;
    if (info->type != STATE_INPUT_CHARACTER && info->prompt) {
        if (closurep(info->prompt)) prompt = funcall(info->prompt);
        else prompt = info->prompt;
        if (prompt) message("input", D_ANSI->parse(prompt), this_object());
    }
    // if (info->type == STATE_INPUT_CHARACTER) {
    //     efun::get_char((: dispatch_input :), info->secure | 2);
    // } else {
        efun::input_to(#'dispatch_input, info->secure | 2); // '
    // }
}

private int create_handler () {
    this_object()->shell_start();

    if (!sizeof(stack)) {
        message("input", "Unable to process input.\n", this_object());
        destruct(this_object());
        return 1;
    }
    return 0;
}

private struct Input get_top_handler (int require_handler) {
    int some_popped = 0;

    while (sizeof(stack)) {
        struct Input info;

        info = stack[<1];
        if (!(closurep(info->inputFn) & FP_OWNER_DESTED)) {
            if (some_popped && info->callbackFn)
            funcall(info->callbackFn);
            return info;
        }

        stack = stack[0..<2];
        some_popped = 1;
    }

    if (!require_handler || create_handler()) return 0;
    return stack[<1];
}

private struct Input get_bottom_handler () {
    while (sizeof(stack)) {
        struct Input info;
        info = stack[0];
        if (!(closurep(info->inputFn) & FP_OWNER_DESTED)) return info;
        stack = stack[1..];
    }

    if (create_handler()) return 0;
    return stack[0];
}