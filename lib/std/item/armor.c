inherit STD_ITEM;
inherit M_BONUS;

nosave string *__Limbs;
nosave object __Worn;
nosave int __AC;

object query_worn ();

int is_armor () {
    return 1;
}

varargs string query_short (string defaultANSI) {
    string short = ::query_short(defaultANSI);

    if (query_worn()) {
        short += " (worn)";
    }

    return short;
}

void set_limbs (string *limbs) {
    __Limbs = limbs;
}
string *query_limbs () {
    return __Limbs;
}

object query_worn () {
    return __Worn;
}
void set_worn (object wearer) {
    __Worn = wearer;
    if (__Worn) {
        apply_bonus(__Worn);
    }
}
void remove_worn (object wearer) {
    if (__Worn && wearer && wearer == __Worn) {
        remove_bonus(__Worn);
    }
    __Worn = 0;
}

int query_ac () {
    return __AC;
}
void set_ac (int ac) {
    __AC = ac;
}

mixed direct_wear_obj (mixed args...) {
    object po = previous_object();
    return environment() == po && !__Worn;
}
mixed direct_unwear_obj (mixed args...) {
    object po = previous_object();
    return environment() == po && __Worn == po && member_array(this_object(), po->query_all_armor()) > -1;
}

mixed direct_drop_obj (mixed args...) {
    return (environment() == previous_object()) && !__Worn;
}