inherit "/std/shadow.c";

nosave private int __Prompts;

void handle_input(string _str) {}
// an input handler that is lost when this mock is destructed
function query_input_fn() {
    return (: handle_input :);
}

string query_prompt() {
    __Prompts++;
    return "mock> ";
}
int query_prompts() {
    return __Prompts;
}

// override from /std/user/shell.c
void shell_start() {
    query_shadow()->input_push((: handle_input :), (: query_prompt :));
}
