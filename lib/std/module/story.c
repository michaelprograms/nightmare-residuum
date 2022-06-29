private int __Delay = 3;

void story_action (object target, string line, int final);
void story_action_final (object target);

/* ----- ----- */

mixed *query_story_lines (object target) {
    return ({ }); // Override
}

/* ----- ----- */

void set_story_delay (int d) {
    __Delay = d;
}
int query_story_delay () {
    return __Delay;
}

/* ----- ----- */

void story_action (object target, mixed line, string *lines) {
    if (!target || (environment(target) != this_object() && environment(target) != environment(this_object()))) return;

    if (stringp(line)) {
        message("story", line + "\n", target);
    } else if (functionp(line)) {
        evaluate(line, target);
    }

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
    mixed *lines;
    if (!target || (environment(target) != this_object() && environment(target) != environment(this_object()))) return;
    lines = query_story_lines(target);
    call_out((: story_action, target, lines[0], lines[1..] :), __Delay);
}