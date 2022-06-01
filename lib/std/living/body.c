#include "living.h"

private string __Gender = "neither";
private string __Species = "unknown";
private int __Level = 0;
mapping __Limbs = ([]);
private nosave mapping __Wielded = ([]);
private nosave mapping __Worn = ([]);

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

    if (!stringp(species)) raise_error("Bad argument 1 to body->set_species");

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
}

/* ----- limbs ----- */

protected void update_limbs () {
    __Limbs = D_SPECIES->setup_body(this_object());
}
string *query_limbs () {
    return keys(__Limbs || ([ ]));
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

/* ----- wearing ----- */

int query_limb_armor (string limb) {
    int ac = 0;

    if (!mappingp(__Worn)) __Worn = ([ ]);

    foreach (object ob in __Worn[limb] || ({})) {
        ac += ob->query_ac();
    }

    return ac;
}
object *query_all_armor () {
    object *worn = ({ });

    if (!mappingp(__Worn)) __Worn = ([ ]);

    foreach (string limb in keys(__Worn)) {
        foreach (object ob in __Worn[limb]) {
            if (member(ob, worn) == -1) {
                worn += ({ ob });
            }
        }
    }
    return worn;
}
int query_can_wear_armor (object ob) {
    if (!mappingp(__Worn)) __Worn = ([ ]);

    foreach (string limb in ob->query_limbs()) {
        if (!pointerp(__Worn[limb])) continue;
        foreach (object worn in __Worn[limb]) {
            if (ob->query_type() == worn->query_type()) {
                return 0;
            }
        }
    }
    return 1;
}

varargs mixed handle_wear (object ob) {
    if (!mappingp(__Worn)) __Worn = ([ ]);

    if (ob->query_worn()) return "You are already wearing " + ob->query_name() + ".";
    if (!query_can_wear_armor(ob)) return "You are already wearing " + ob->query_type() + ".";

    foreach (string limb in ob->query_limbs()) {
        if (!pointerp(__Worn[limb])) __Worn[limb] = ({ });
        __Worn[limb] += ({ ob });
    }
    ob->set_worn(this_object());
    return 1;
}
varargs mixed handle_unwear (object ob) {
    if (!mappingp(__Worn)) __Worn = ([]);

    if (!ob->query_worn()) return "You are not wearing " + ob->query_name() + ".";

    foreach (string limb in ob->query_limbs()) {
        if (!pointerp(__Worn[limb])) __Worn[limb] = ({ });
        if (member(ob, __Worn[limb]) > -1) {
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

varargs mixed handle_wield (object ob, string limb) {
    if (!mappingp(__Wielded)) __Wielded = ([ ]);

    if (ob->query_wielded()) return "You are already wielding " + ob->query_name() + ".";
    if (!limb) {
        string *limbs = query_wieldable_limbs();
        if (sizeof(limbs)) limb = limbs[0];
    }
    if (!limb) return "You are out of limbs to wield " + ob->query_name() + ".";

    ob->set_wielded(this_object());
    __Wielded[limb] = ob;
    return 1;
}
varargs mixed handle_unwield (object ob, string limb) {
    if (!mappingp(__Wielded)) __Wielded = ([ ]);

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