#include "living.h"

private string __Gender = "neither";
private string __Species;
private int __Level = 0;
mapping __Limbs = ([]);
private nosave mapping __Wielded = ([]);

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
    return keys(__Limbs || ([]));
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

/* ----- weapons ----- */

string *query_wieldable_limbs () {
    string *wieldable = ({ });

    foreach (string l in query_limbs()) {
        if (__Limbs[l]["limbtype"] == "WIELD" && !__Wielded[l]) wieldable += ({ l });
    }

    return wieldable;
}
object query_wielded (string limb) {
    if (limb && __Limbs[limb]["limbtype"] == "WIELD" && __Wielded[limb]) return __Wielded[limb];
    return 0;
}
object *query_wielded_weapons () {
    object *weapons = ({ });
    foreach (string l in query_limbs()) {
        if (__Limbs[l]["limbtype"] == "WIELD" && __Wielded[l]) weapons += ({ query_wielded(l) });
    }
    return weapons;
}

varargs int handle_wield (object weapon, string limb) {
    if (!limb) {
        string *limbs = query_wieldable_limbs();
        if (sizeof(limbs)) limb = limbs[0];
    }
    if (!limb) return 0;
    weapon->set_wielded(this_object());
    if (!mapp(__Wielded)) __Wielded = ([]);
    __Wielded[limb] = weapon;
    return 1;
}
varargs int handle_unwield (object weapon, string limb) {
    if (!mapp(__Wielded)) __Wielded = ([]);
    if (!limb) {
        foreach (string l in query_limbs()) {
            if (__Limbs[l]["limbtype"] == "WIELD" && __Wielded[l] == weapon) {
                limb = l;
                break;
            }
        }
    }
    if (!limb) return 0;
    weapon->set_wielded(0);
    __Wielded[limb] = 0;
    return 1;
}