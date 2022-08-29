#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

mixed can_wear () {
    return "Wear what?";
}

void do_wear_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_wear(ob)) == 1) {
        return;
    } else if (stringp(result)) {
        message("verb", result + "\n", po);
    } else {
        message("verb", "You try to wear " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_wear_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_wear_obj(ob, str);
    }
}