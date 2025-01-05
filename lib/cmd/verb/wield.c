#include <verb.h>

inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", "OBS", }));
    set_requirements(REQUIREMENT_BUSY | REQUIREMENT_DISABLE);
    set_syntax("wield [item]");
    set_help_text("Your character will attempt to wield the provided item.");
    set_help_similar(({ "unwear", "unwield", "wear", }));
}

mixed can_wield () {
    return "Wield what?";
}

/**
 * Equip an unwielded weapon item to a living body.
 *
 * @param {STD_WEAPON} ob the weapon item to be equipped
 * @param str the input text from the player (ex, 'staff' or 'sword')
 */
void do_wield_obj (object ob, string str) {
    mixed result;
    object po = previous_object();

    if ((result = po->handle_wield(ob)) == 1) {
        return;
    } else if (stringp(result)) {
        message("action", result, po);
    } else {
        message("action", "You try to wield " + ob->query_name() + " but fail for some reason.", po);
    }
}
void do_wield_obs (mixed *info, string str) {
    foreach (object ob in info) {
        do_wield_obj(ob, str);
    }
}