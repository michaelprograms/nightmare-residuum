#include "living.h"

nosave private object *__Hostiles;

void create () {
    __Hostiles = ({});
}

int add_hostile (object ob) {
    if (!ob || !ob->is_living() || member_array(ob, __Hostiles) > -1) {
        return 0;
    }
    __Hostiles += ({ ob });
    return 1;
}
int remove_hostile (object ob) {
    if (!ob || !ob->is_living() || member_array(ob, __Hostiles) == -1) {
        return 0;
    }
    __Hostiles -= ({ ob });
    return 1;
}
object *query_hostiles () {
    __Hostiles = filter_array(__Hostiles, (: !undefinedp($1) :));
    return __Hostiles;
}