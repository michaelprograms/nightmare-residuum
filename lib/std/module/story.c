nosave private int __Delay = 3;
nosave private mixed *__Lines = ({ });

void story_action (object target, string line, int final);
void story_action_final (object target);

/* ----- ----- */

mixed *query_story_lines () {
    return __Lines;
}
void set_story_lines (mixed *lines) {
    __Lines = lines;
}

/* ----- ----- */

void set_story_delay (int d) {
    __Delay = d;
}
int query_story_delay () {
    return __Delay;
}

/* ----- ----- */

void story_action (object target, mixed line, mixed *lines) {
    if (!target || (environment(target) != this_object() && environment(target) != environment())) {
        return;
    }

    if (stringp(line)) {
        message("action", line, target);
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
    if (!target || (environment(target) != this_object() && environment(target) != environment()) || !sizeof(__Lines)) {
        return;
    }
    call_out((: story_action, target, __Lines[0], __Lines[1..] :), __Delay);
}