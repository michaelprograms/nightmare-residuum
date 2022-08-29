#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

mixed can_unwield () {
    return "Unwield what?";
}

void do_unwield_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_unwield(ob)) == 1) {
        return;
    } else if (stringp(result)) {
        message("verb", result + "\n", po);
    } else {
        message("verb", "You try to unwield " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_unwield_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_unwield_obj(ob, str);
    }
}