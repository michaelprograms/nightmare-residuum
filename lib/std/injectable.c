inherit STD_ITEM;

nosave private string __Type = "";
nosave private int __Strength = 0;

int is_injectable () { return 1; }

void set_type (string t) {
    __Type = t;
}
string query_type () {
    return __Type;
}
void set_strength (int s) {
    __Strength = s;
}
int query_strength () {
    return __Strength;
}

void handle_inject (object target) {
    target->add_injection(__Type, __Strength);
    target->set_disable(1);
    // @TODO add empty container
    handle_remove();
}

/* ----- parser applies ----- */

mixed direct_inject_obj (mixed args...) {
    return environment() == previous_object();
}
mixed direct_inject_obj_into_liv (mixed args...) {
    return environment() == previous_object();
}