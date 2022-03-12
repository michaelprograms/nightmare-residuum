inherit STD_ITEM;

nosave string __Type, *__Limbs;
nosave object __Worn;

object query_worn ();

string query_short () {
    string short = ::query_short();

    if (query_worn()) short += " (worn)";

    return short;
}

void set_type (string type) {
    __Type = type;
}
string query_type () {
    return __Type;
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
}

mixed direct_wear_obj (object ob, string str) {
    object po = previous_object();
    return environment() == po && !__Worn && po->query_can_wear_armor(this_object());
}
mixed direct_unwear_obj (object ob, string str) {
    object po = previous_object();
    return environment() == po && __Worn == po && member_array(this_object(), po->query_worn_armor()) > -1;
}