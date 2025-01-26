string query_name();

void add_parse_rule (string name, string rule) {
    efun::parse_add_rule(name, rule);
}
void add_parse_synonym (string synonym, string name, string rule) {
    efun::parse_add_synonym(synonym, name, rule);
}

varargs void add_rules (string *rules, string *syns) {
    string name = this_object()->query_name();
    foreach (string rule in rules) {
        this_object()->add_parse_rule(name, rule);
        if (arrayp(syns)) {
            foreach (string syn in syns) {
                this_object()->add_parse_synonym(syn, name, rule);
            }
        }
    }
}

void create () {
    efun::parse_init();
}