#include "living.h"

private string __Gender = "neither";
private string __Species = "unknown";
private int __Level = 0;
mapping __Limbs = ([ ]);
private nosave mapping __Wielded = ([ ]);
private nosave mapping __Worn = ([ ]);

/* ----- gender and species ----- */

string query_gender () {
    return __Gender;
}
void set_gender (string str) {
    switch(str) {
    case "male": case "female": case "neither":
        __Gender = str;
        break;
    default:
        __Gender = "none";
    }
}

string query_species () {
    return stringp(__Species) ? __Species : "unknown";
}
void set_species (string species) {
    // @TODO security

    if (!stringp(species)) error("Bad argument 1 to body->set_species");

    __Species = species;
    update_limbs();
}

/* ----- level and experience ----- */

int query_level () {
    return __Level;
}
void set_level (int l) {
    if (l < 0) {
        l = abs(l);
    }
    __Level = l;

    update_limbs();
}

/* ----- limbs ----- */

protected void update_limbs () {
    mapping newLimbs = D_SPECIES->setup_body(this_object());

    foreach (string limb,mapping value in newLimbs || ([])) {
        if (__Limbs[limb]) {
            value["damage"] = __Limbs[limb]["damage"];
            value["status"] = __Limbs[limb]["status"];
        }
        __Limbs[limb] = value;
    }
}
string *query_limbs () {
    string *limbs = keys(__Limbs || ([ ]));
    limbs = filter_array(limbs, (: __Limbs[$1]["status"] != "severed" :));
    return limbs;
}
string *query_severed_limbs () {
    string *limbs = keys(__Limbs || ([ ]));
    limbs = filter_array(limbs, (: __Limbs[$1]["status"] == "severed" :));
    return limbs;
}
mapping query_limb (string limb) {
    if (limb && __Limbs[limb]) return __Limbs[limb];
    return 0;
}
string query_random_limb () {
    string *limbs = query_limbs();
    if (sizeof(limbs)) return limbs[random(sizeof(limbs))];
    else return 0;
}

void handle_limb_sever (string limb) {

    // allow fatal limbs to be severed multiple times
    if (__Limbs[limb]["status"] == "severed" && __Limbs[limb]["type"] != "FATAL") {
        return;
    }
    __Limbs[limb]["status"] = "severed";
    __Limbs[limb]["damage"] = -1;
    if (__Limbs[limb]["type"] == "FATAL") {
        message("combat action", "Your "+limb+" receives a mortal blow!\n", this_object());
        message("combat action", possessive_noun(this_object())+" "+limb+" receives a mortal blow!\n", environment(), this_object());
    } else {
        message("combat action", "Your "+limb+" is severed!\n", this_object());
        message("combat action", possessive_noun(this_object())+" "+limb+" is severed!\n", environment(), this_object());
    }

    if (__Limbs[limb]["attach"] && __Limbs[limb]["attach"] != limb) {
        handle_limb_sever(__Limbs[limb]["attach"]);
    }
}

void handle_limb_restore (string limb) {
    if (__Limbs[limb]["status"] != "severed") {
        return;
    }
    map_delete(__Limbs[limb], "status");
    __Limbs[limb]["damage"] = 0;
}

/* ----- damage ----- */

varargs int handle_damage (int damage, string limb, object source) {
    // @TODO source
    add_hp(-damage);
    if (query_max_hp() < query_hp()) set_hp(query_max_hp());

    if (stringp(limb) && limb != "" && this_object()->query_limb(limb)) {
        int limbDamagePct;

        __Limbs[limb]["damage"] += damage / 2;
        if (__Limbs[limb]["damage"] < 0) {
            __Limbs[limb]["damage"] = 0;
        }
        limbDamagePct = __Limbs[limb]["damage"]*100/__Limbs[limb]["maxdamage"];

        if (limbDamagePct >= 100) {
            handle_limb_sever(limb);
        } else if (limbDamagePct >= 75) {
            message("combat alert", "Your "+limb+" is badly damaged!", this_object());
        } else if (limbDamagePct >= 50) {
            message("combat alert", "Your "+limb+" is injured!", this_object());
        }
    }

    if (this_object()->is_character()) {
        message("system", sprintf("hp: %d    sp: %d    mp: %d\n", query_hp(), query_sp(), query_mp()), this_object());
    }
    check_lifesigns(source);
    return damage;
}

/* ----- wearing ----- */

int query_limb_armor (string limb) {
    int ac = 0;

    if (!mapp(__Worn)) __Worn = ([ ]);

    foreach (object ob in __Worn[limb] || ({})) {
        ac += ob->query_ac();
    }

    return ac;
}
object *query_all_armor () {
    object *worn = ({ });

    if (!mapp(__Worn)) __Worn = ([ ]);

    foreach (string limb in keys(__Worn)) {
        foreach (object ob in __Worn[limb]) {
            if (member_array(ob, worn) == -1) {
                worn += ({ ob });
            }
        }
    }
    return worn;
}
int query_can_wear_armor (object ob) {
    if (!mapp(__Worn)) __Worn = ([ ]);

    foreach (string limb in ob->query_limbs()) {
        if (!arrayp(__Worn[limb])) continue;
        foreach (object worn in __Worn[limb]) {
            if (ob->query_type() == worn->query_type()) {
                return 0;
            }
        }
    }
    return 1;
}

varargs mixed handle_wear (object ob) {
    if (!mapp(__Worn)) __Worn = ([ ]);

    if (ob->query_worn()) return "You are already wearing " + ob->query_name() + ".";
    if (!query_can_wear_armor(ob)) return "You are already wearing " + ob->query_type() + ".";

    foreach (string limb in ob->query_limbs()) {
        if (!arrayp(__Worn[limb])) __Worn[limb] = ({ });
        __Worn[limb] += ({ ob });
    }
    ob->set_worn(this_object());
    return 1;
}
varargs mixed handle_unwear (object ob) {
    if (!mapp(__Worn)) __Worn = ([]);

    if (!ob->query_worn()) return "You are not wearing " + ob->query_name() + ".";

    foreach (string limb in ob->query_limbs()) {
        if (!arrayp(__Worn[limb])) __Worn[limb] = ({ });
        if (member_array(ob, __Worn[limb]) > -1) {
            __Worn[limb] -= ({ ob });
        }
    }
    ob->set_worn(0);
    return 1;
}

/* ----- wielding ----- */

string *query_wieldable_limbs () {
    string *wieldable = ({ });
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["type"] == "WIELD" && !__Wielded[l]) wieldable += ({ l });
    }
    return wieldable;
}
object query_wielded (string l) {
    if (l && __Limbs[l]["type"] == "WIELD" && __Wielded[l]) return __Wielded[l];
    return 0;
}
object *query_wielded_weapons () {
    object *weapons = ({ });
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["type"] == "WIELD" && __Wielded[l]) weapons += ({ query_wielded(l) });
    }
    return weapons;
}
string *query_wielded_limbs (object ob) {
    string *limbs = ({ });
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["type"] == "WIELD" && __Wielded[l] == ob) {
            limbs += ({ l });
        }
    }
    return limbs;
}

varargs mixed handle_wield (object ob) {
    string *limbs = query_wieldable_limbs(), *hands;

    if (!mapp(__Wielded)) __Wielded = ([ ]);

    if (ob->query_wielded()) return "You are already wielding " + ob->query_name() + ".";

    if (ob->query_hands() == 1) {
        if (sizeof(limbs)) {
            hands = limbs[0..0];
        }
    } else if (ob->query_hands() == 2) {
        if (sizeof(limbs) > 1) {
            hands = limbs[0..1];
        }
    }
    if (sizeof(hands) < ob->query_hands()) return "You are out of limbs to wield " + ob->query_name() + ".";

    foreach (string limb in hands) {
        __Wielded[limb] = ob;
    }
    ob->set_wielded(this_object());

    return 1;
}
varargs mixed handle_unwield (object ob, string limb) {
    if (!mapp(__Wielded)) __Wielded = ([ ]);

    if (!ob->query_wielded()) return "You are not wielding " + ob->query_name() + ".";
    if (!limb) {
        foreach (string l in query_limbs()) {
            if (__Limbs[l]["type"] == "WIELD" && __Wielded[l] == ob) {
                limb = l;
                break;
            }
        }
    }
    if (!limb) return "You are not wielding anything in your " + limb + ".";

    ob->set_wielded(0);
    __Wielded[limb] = 0;
    return 1;
}

}