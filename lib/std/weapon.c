inherit STD_ITEM;

nosave string __Type;
nosave object __Wielder;

object query_wielded ();

int is_weapon () { return 1; }

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

mixed direct_wield_obj (mixed args...) {
    object po = previous_object();
    return environment() == po && !__Wielder && member(this_object(), po->query_wielded_weapons()) == -1;
}
mixed direct_unwield_obj (mixed args...) {
    object po = previous_object();
    return environment() == po && __Wielder == po && member(this_object(), po->query_wielded_weapons()) > -1;
}