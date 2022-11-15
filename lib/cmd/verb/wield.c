#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("wield [item]");
    set_help_text("Your character will attempt to wield the provided item.");
}

mixed can_wield () {
    return "Wield what?";
}

void do_wield_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_wield(ob)) == 1) {
        return;
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