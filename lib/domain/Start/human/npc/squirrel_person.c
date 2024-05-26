#include "human.h"

inherit STD_NPC;

void create () {
    string name, capName;
    ::create();

    if (random(2)) {
        name = element_of(HUMAN_NAMES[0]);
        set_gender("male");
    } else {
        name = element_of(HUMAN_NAMES[1]);
        set_gender("female");
    }
    capName = capitalize(name);
    set_id(({ name, "human", }));
    set_name(capName);
    set_short(capName + " the Squirrel Person");
    set_long("A human by the name of " + capName + ", covered in squirrels. The rodents seem to like "+objective(query_gender())+" quite a bit.");
    set_level(10);
    set_species("human");

    set_ability_list(({ "zap", "missile", }));
    set_ability_chance(50);

    set_inventory(([
        HUMAN_WEAPON "wooden_staff.c": "wield staff",
    ]));

    parse_init();
    parse_add_rule("adopt", "");
    parse_add_rule("adopt", "STR");
    parse_add_rule("reclaim", "");
    parse_add_rule("reclaim", "STR");
}

/* ----- parser rule: adopt ----- */

mixed can_adopt () {
    return previous_object()->is_character() && environment(previous_object()) == environment();
}
void do_adopt () {
    message("action", "Adopt what?", previous_object());
}
mixed can_adopt_str (mixed args...) {
    return previous_object()->is_character() && environment(previous_object()) == environment();
}
void do_adopt_str (mixed args...) {
    object po = previous_object(), pet;
    string str;

    if (sizeof(args)) {
        str = args[0];
    }

    if (member_array(str, ({ "squirrel" })) == -1) {
        return do_adopt();
    }

    if (D_CHARACTER->query_exists(po->query_key_name(), "pet")) {
        message("action", "You have already adopted a squirrel.", po);
        return;
    }

    pet = new("/std/npc/pet.c");
    pet->set_save_path(D_CHARACTER->query_save_path(po->query_key_name(), "pet"));
    pet->set_owner(po);
    pet->set_species("squirrel");
    pet->set_gender(element_of(({ "male", "female" })));
    pet->set_level(1);
    pet->set_id(({ "squirrel", "pet" }));
    pet->set_adjective(({ "tame" }));
    pet->set_name(possessive_noun(po->query_cap_name())+" squirrel");
    pet->set_short(pet->query_cap_name());
    pet->set_long("A squirrel tamed by the Squirrel Person, "+subjective(pet)+" is very friendly.");
    pet->set_following(1);
    pet->set_ability_list(({ "bite", "scratch", }));
    pet->set_ability_chance(25);
    pet->save_data();

    handle_command("say You want to take a squirrel? Here you go... I expect you to take good care of "+objective(pet)+"!");
    message("action", "A squirrel walks over to your side.", po);
    message("action", "A squirrel walks over to "+possessive_noun(po->query_cap_name())+" side.", environment(po), po);
    pet->handle_move(environment(po));
}

/* ----- parser rule: reclaim ----- */

mixed can_reclaim () {
    return previous_object()->is_character() && environment(previous_object()) == environment();
}
void do_reclaim () {
    message("action", "Reclaim what?", previous_object());
}
mixed can_reclaim_str (mixed args...) {
    return previous_object()->is_character() && environment(previous_object()) == environment();
}
void do_reclaim_str (mixed args...) {
    object po = previous_object(), pet;
    string str;

    if (sizeof(args)) {
        str = args[0];
    }

    if (member_array(str, ({ "squirrel" })) == -1) {
        return do_reclaim();
    }

    if (!D_CHARACTER->query_exists(po->query_key_name(), "pet")) {
        message("action", "You do not have a squirrel to reclaim.", po);
        return;
    }

    // setup existing squirrel
    pet = new("/std/npc/pet.c");
    pet->set_save_path(D_CHARACTER->query_save_path(po->query_key_name(), "pet"));
    pet->restore_data();
    pet->set_owner(po);
    pet->set_id(({ "squirrel", "pet" }));
    pet->set_adjective(({ "tame" }));
    pet->set_name(possessive_noun(po->query_cap_name())+" squirrel");
    pet->set_short(pet->query_cap_name());
    pet->set_ability_list(({ "bite", "scratch", }));
    pet->set_ability_chance(25);
    pet->set_long("A squirrel tamed by the Squirrel Person, "+subjective(pet)+" is very friendly.");

    handle_command("say Here "+subjective(pet)+" is, take good care of "+objective(pet)+".");
    message("action", "Your squirrel walks over to your side.", po);
    message("action", possessive_noun(po->query_cap_name())+" squirrel walks over to "+possessive(po)+" side.", environment(po), po);
    pet->handle_move(environment(po));
}