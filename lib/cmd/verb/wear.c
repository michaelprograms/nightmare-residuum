#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("wear [item]");
    set_help_text("Your character will attempt to wear the provided item.");
    set_help_similar(({ "unwear", "unwield", "wield", }));
}

mixed can_wear () {
    return "Wear what?";
}

/**
 * Wear an unworn armor item to a living body.
 *
 * @param {STD_ARMOR} ob the armor item to be equipped
 * @param str the input text from the player (ex, 'all' or 'helmet')
 */
void do_wear_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_wear(ob)) == 1) {
        return;
    } else if (stringp(result)) {
        message("action", result, po);
    } else {
        message("action", "You try to wear " + ob->query_name() + " but fail for some reason.", po);
    }
}
void do_wear_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_wear_obj(ob, str);
    }
}