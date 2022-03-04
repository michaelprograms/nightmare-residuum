inherit STD_ITEM;

nosave string __Type;
nosave object __Wielder;

void set_type (string type) {
    __Type = type;
}
string query_type () {
    return __Type;
}

object query_wielded () {
    return __Wielder;
}
void set_wielded (object wielder) {
    __Wielder = wielder;
}

mixed direct_wield_obj (object ob, string str) {
    string *wieldable = this_character()->query_wieldable_limbs();

    if (!sizeof(wieldable)) return "You are out of free hands.";

    if (environment() != previous_object()) return 0;
    return 1;
}