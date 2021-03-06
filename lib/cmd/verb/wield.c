#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

mixed can_wield () {
    return "Wield what?";
}

void do_wield_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_wield(ob)) == 1) {
        string limbConj = conjunction(po->query_wielded_limbs(ob));
        message("verb", "You wield " + ob->query_name() + " in your " + limbConj + ".\n", po);
        message("verb", po->query_cap_name() + " wields " + ob->query_name() + " in " + possessive(po) + " " + limbConj + ".\n", environment(po), po);
    } else if (stringp(result)) {
        message("verb", result + "\n", po);
    } else {
        message("verb", "You try to wield " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_wield_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_wield_obj(ob, str);
    }
}