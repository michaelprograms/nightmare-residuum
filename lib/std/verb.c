#include <verb.h>

inherit STD_COMMAND;
inherit M_CLEAN;

nosave private int __Requirements = REQUIREMENT_NONE;
nosave private string __Verb;

string query_name () {
    return __Verb;
}

protected varargs void add_rules (mixed *rules, mixed *syns) {
    foreach (string rule in rules) {
        parse_add_rule(__Verb, rule);
        if (!undefinedp(syns)) {
            foreach (string syn in syns) {
                parse_add_synonym(syn, __Verb, rule);
            }
        }
    }
}

void set_requirements (int requirements) {
    __Requirements = requirements;
}
int query_requirements () {
    return __Requirements;
}

mixed check_busy () {

    if (previous_object()->query_busy() > 0) {
        return "You are too busy to do that right now.";
    } else {
        return 1;
    }
}
mixed check_disable () {;
    if (previous_object()->query_disable() > 0) {
        return "You are not able to do that right now.";
    } else {
        return 1;
    }
}

/* ----- applies ----- */

void create () {
    __Verb = split_path(base_name())[1];
    set_no_clean(1);
    parse_init();
}

mixed can_verb_rule (mixed args...) {
    mixed tmp;
    if ((__Requirements & REQUIREMENT_BUSY) && (tmp = check_busy()) != 1) return tmp;
    if ((__Requirements & REQUIREMENT_DISABLE) && (tmp = check_disable()) != 1) return tmp;
    return 1;
}