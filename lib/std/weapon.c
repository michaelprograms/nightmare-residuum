inherit STD_ITEM;

nosave string __Type;
nosave object __Wielder;

object query_wielded ();

string query_short () {
    string short = ::query_short();

    if (query_wielded()) short += " (wielded)";

    return short;
}

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
    return environment() == previous_object();
}
mixed direct_unwield_obj (object ob, string str) {
    object po = previous_object();
    return environment() == po && __Wielder == po && member_array(this_object(), po->query_wielded_weapons()) > -1;
}