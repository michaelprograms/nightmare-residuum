nosave mapping __Weapons = ([ ]);

void set_weapons (mapping weapons) {
    /* Data format:
    ([
        "blade|blunt": ({ 1, 2 }),
        "brawl": ({ 1 }),
    ])
    */
    // Validate weapons
    if (undefinedp(weapons) || !mapp(weapons)) {
        error("Bad argument 1 to weapon->set_weapons");
    }
    foreach (string key,int *hands in weapons) {
        if (member_array(key, ({ "blade", "blunt", "brawl" })) == -1) {
            error("Bad argument (keys) to weapon->set_weapons");
        }
        foreach (int hand in hands) {
            if (hand < 1 || hand > 2) {
                error("Bad argument (values) to weapon->set_weapons");
            }
        }
    }
    __Weapons = weapons;
}
mapping query_weapons () {
    return __Weapons;
}
/**
 * Find the best wielded weapon according to this ability's requirements.
 *
 * @param {STD_LIVING} source who is using this ability
 * @returns {STD_WEAPON} the best weapon
 */
object query_best_weapon (object source) {
    string *types, t;
    object weapon;
    /** @type {STD_WEAPON*} weapons */
    object *weapons;
    int wc;

    types = keys(__Weapons);
    weapons = source->query_wielded_weapons();
    foreach (object w in weapons) {
        if (sizeof(types)) {
            t = w->query_type();
            if (member_array(t, types) == -1) {
                continue;
            }
            if (member_array(w->query_hands(), __Weapons[t]) == -1) {
                continue;
            }
        }
        if (w->query_wc() <= wc) {
            continue;
        }
        wc = w->query_wc();
        weapon = w;
    }

    return weapon;
}