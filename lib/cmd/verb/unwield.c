#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("unwield [item]");
    set_help_text("Your character will attempt to unwield the provided item.");
    set_help_similar(({ "unwear", "wear", "wield", }));
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
        message("action", result, po);
    } else {
        message("action", "You try to unwield " + ob->query_name() + " but fail for some reason.", po);
    }
}
void do_unwield_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_unwield_obj(ob, str);
    }
}