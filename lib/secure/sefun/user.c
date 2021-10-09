#include "sefun.h"

object this_account () {
    object a;
    if (this_user() && (a = this_user()->query_account())) return a;
    else return 0;
}
object this_character () {
    object c;
    if (this_user() && (c = this_user()->query_character())) return c;
    else return 0;
}
object find_character (string name) {
    object *results = filter_array(children(STD_CHARACTER), (: $1 && $1->query_key_name() == $2:), sanitize_name(name));
    return sizeof(results) ? results[0] : 0;
}