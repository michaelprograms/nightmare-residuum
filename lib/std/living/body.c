#include "living.h"

private string __Gender = "neither";
private string __Species;
private int __Level = 0;
mapping __Limbs = ([]);

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
    switch(__Species) {
    case "human":
        __Limbs = ([
            "head": ([]),
            "torso": ([]),
            "left arm": ([]),
            "right arm": ([]),
            "left hand": ([ "wieldable": 1, "wielded": 0, ]),
            "right hand": ([ "wieldable": 1, "wielded": 0, ]),
            "left leg": ([]),
            "right leg": ([]),
            "left foot": ([]),
            "right foot": ([]),
        ]);
        break;
    case "mouse":
        __Limbs = ([
            "head": ([]),
            "torso": ([]),
            "left fore leg": ([]),
            "right fore leg": ([]),
            "left fore paw": ([]),
            "right fore paw": ([]),
            "left rear leg": ([]),
            "right rear leg": ([]),
            "left rear paw": ([]),
            "right rear paw": ([]),
        ]);
        break;
    }
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
    if (sizeof(limbs)) {
        return limbs[random(sizeof(limbs))];
    } else return 0;
}

string *query_wieldable_limbs () {
    string *wieldable = ({ });

    foreach (string l in query_limbs()) {
        if (__Limbs[l]["wieldable"] && !__Limbs[l]["wielded"]) wieldable += ({ l });
    }

    return wieldable;
}

object query_wielded (string limb) {
    if (limb && __Limbs[limb] && __Limbs[limb]["wieldable"]) return __Limbs[limb]["wielded"];
    return 0;
}

object *query_wielded_weapons () {
    object *weapons = ({ });

    foreach (string l in query_limbs()) {
        if (__Limbs[l]["wieldable"] && __Limbs[l]["wielded"]) weapons += ({ query_wielded(l) });
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
    __Limbs[limb]["wielded"] = weapon;
    return 1;
}
varargs int handle_unwield (object weapon, string limb) {
    if (!limb) {
        foreach (string l in query_limbs()) {
            if (__Limbs[l]["wieldable"] && __Limbs[l]["wielded"] == weapon) {
                limb = l;
                break;
            }
        }
    }
    if (!limb) return 0;
    weapon->set_wielded(0);
    __Limbs[limb]["wielded"] = 0;
    return 1;
}