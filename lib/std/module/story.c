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

void story_action (object target, string line, int final) {
    if (!target || environment(target) != this_object()) return;
    message("story", line + "\n", target);
    if (final) story_action_final(target);
}

void story_action_final (object target) {
    /* Override */
}

void story_start (object target) {
    string *lines;

    if (!target || environment(target) != this_object()) return;
    lines = query_story_lines(target);
    for (int i = 0; i < sizeof(lines); i ++) {
        call_out((: story_action, target, lines[i], (i+1 == sizeof(lines)) :), (i+1) * __Delay);
    }
}