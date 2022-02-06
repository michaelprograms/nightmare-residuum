private int __Delay = 3;

void story_action (object target, string line, int final);
void story_action_final (object target);

/* ----- ----- */

string *query_story_lines (object target) {
    return ({}); // Override
}

/* ----- ----- */

void set_delay (int d) {
    __Delay = d;
}
int query_delay () {
    return __Delay;
}

/* ----- ----- */

void story_action (object target, string line, string *lines) {
    if (!target || environment(target) != this_object()) return;
    message("story", line + "\n", target);
    if (!sizeof(lines)) {
        story_action_final(target);
    } else {
        call_out((: story_action, target, lines[0], lines[1..] :), __Delay);
    }
}

void story_action_final (object target) {
    /* Override */
}

void story_start (object target) {
    string *lines;

    if (!target || environment(target) != this_object()) return;
    lines = query_story_lines(target);
    call_out((: story_action, target, lines[0], lines[1..] :), __Delay);
}