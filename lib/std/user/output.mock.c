inherit "/std/shadow.c";

nosave private function __Page, __Prompt;
function query_page() {
    return __Page;
}
function query_prompt() {
    return __Prompt;
}

// override from /std/user/account.c, one line per page keeps the pager open
mixed query_setting(string key) {
    return 1;
}
// override from /std/user/input.c
varargs void input_push(function inputFn, mixed prompt) {
    __Page = inputFn;
    __Prompt = prompt;
}
