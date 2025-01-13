inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "OBJ", }));
    set_syntax("harvest [resource]");
    set_help_text("Your character will attempt to harvest the provided resource from your environment.");
}

/* ----- rule: "" ----- */

mixed can_harvest () {
    return "Harvest what?";
}

/* ----- rule: "OBJ" ----- */

mixed can_harvest_obj (object ob, string str) {
    return 1;
}
/**
 * Harvest a resource from a harvestable item.
 *
 * @param {"/std/resource/harvestable.c"} ob the item to harvest
 * @param str the text input
 */
void do_harvest_obj (object ob, string str) {
    ob->handle_harvest(previous_object());
}