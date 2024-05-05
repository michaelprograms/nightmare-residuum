inherit "/std/shadow.c";


/* ----- used by tests ----- */

string query_name () {
    return "mockparse";
}

private nosave mixed *__Rules = ({ });
mixed *query_rules () {
    return __Rules;
}
private nosave mixed *__Synonym = ({ });
mixed *query_synonym () {
    return __Synonym;
}

/* ----- shadowed parser functions ----- */

void add_parse_rule (string name, string rule) {
    __Rules += ({ ({ name, rule }) });
}
void add_parse_synonym (string synonym, string name, string rule) {
    __Synonym += ({ ({ synonym, name, rule }) });
}