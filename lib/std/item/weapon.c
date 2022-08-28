inherit STD_ITEM;

nosave private string __Type;
nosave private object __Wielder;
nosave private int __Hands = 1;
nosave private int __WC;

object query_wielded ();

int is_weapon () { return 1; }

varargs string query_short (string defaultANSI) {
    string short = ::query_short(defaultANSI);

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

int query_hands () {
    if (!__Hands) {
        __Hands = 1;
    }
    return __Hands;
}
void set_hands (int hands) {
    if (!intp(hands)) error("Bad argument 1 to weapon->set_hands");
    if (hands < 1 || hands > 2) error("Bad argument 1 to weapon->set_hands");
    __Hands = hands;
}

int query_wc () {
    return __WC;
}
void set_wc (int wc) {
    __WC = wc;
}

mixed direct_wield_obj (mixed args...) {
    object po = previous_object();
    return environment() == po && !__Wielder && member_array(this_object(), po->query_wielded_weapons()) == -1;
}
mixed direct_unwield_obj (mixed args...) {
    object po = previous_object();
    return environment() == po && __Wielder == po && member_array(this_object(), po->query_wielded_weapons()) > -1;
}