#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
}

mixed can_unwear () {
    return "Unwear what?";
}

void do_unwear_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_unwear(ob)) == 1) {
        string limbConj = conjunction(ob->query_limbs());
        message("verb", "You remove " + ob->query_name() + " from your " + limbConj + ".\n", po);
        message("verb", po->query_cap_name() + " removes " + ob->query_name() + " from " + possessive(po) + " " + limbConj + ".\n", environment(po), po);
    } else if (stringp(result)) {
        message("verb", result + "\n", po);
    } else {
        message("verb", "You try to remove " + ob->query_name() + " but fail for some reason.\n", po);
    }
}
void do_unwear_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_unwear_obj(ob, str);
    }
}