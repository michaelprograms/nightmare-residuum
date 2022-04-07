inherit M_CLEAN;

#define REQUIREMENT_BUSY        (1 << 0)
#define REQUIREMENT_DISABLE     (1 << 1)

nosave private int __Requirements = 0;
nosave private string __Verb = split_path(file_name())[1];

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

mixed can_verb_rule (string verb, string rule) {
    if ((__Requirements & REQUIREMENT_BUSY)) return "You are too busy to do that right now.\n";
    if ((__Requirements & REQUIREMENT_DISABLE)) return "You are not able to do that right now.\n";
    return 1;
}

void create () {
    set_no_clean(1);
    parse_init();
}