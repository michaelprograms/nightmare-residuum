#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("unwear [item]");
    set_help_text("Your character will attempt to remove the provided item.");
}

mixed can_unwear () {
    return "Unwear what?";
}

void do_unwear_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_unwear(ob)) == 1) {
        return;
    } else if (stringp(result)) {
        message("action", result, po);
    } else {
        message("action", "You try to remove " + ob->query_name() + " but fail for some reason.", po);
    }
}
void do_unwear_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_unwear_obj(ob, str);
    }
}