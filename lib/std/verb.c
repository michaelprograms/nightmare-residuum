inherit M_CLEAN;

#define REQUIRE_SIGHT       1
#define REQUIRE_ALIVE       2
#define REQUIRE_THOUGHT     4
#define TRY_TO_ACQUIRE      8

nosave private string __Verb = split_path(file_name())[1];
// nosave private int __Requirements = REQUIRE_SIGHT | REQUIRE_ALIVE | REQUIRE_THOUGHT;

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

mixed check_requirements () {
    // @TODO
    // mixed tmp;
    // if ((__Requirements & NEED_TO_SEE) && (tmp = check_vision()) != 1) return tmp;
    // if ((__Requirements & NEED_TO_BE_ALIVE) && (tmp = check_ghost()) != 1) return tmp;
    // if ((__Requirements & NEED_TO_THINK)) return check_condition();
     return 1;
}

mixed can_verb_rule (string verb, string rule) {
    return check_requirements();
}

void create () {
    set_no_clean(1);
    parse_init();
}