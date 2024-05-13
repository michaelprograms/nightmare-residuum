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
        error("Bad argument 1 to ability->set_weapons");
    }
    foreach (string key,int *hands in weapons) {
        if (member_array(key, ({ "blade", "blunt", "brawl" })) == -1) {
            error("Bad argument (keys) to ability->set_weapons");
        }
        foreach (int hand in hands) {
            if (hand < 1 || hand > 2) {
                error("Bad argument (values) to ability->set_weapons");
            }
        }
    }
    __Weapons = weapons;
}
mapping query_weapons () {
    return __Weapons;
}
object query_best_weapon (object source) {
    object weapon;
    string *types, t;
    int wc;

    types = keys(__Weapons);

    foreach (object w in source->query_wielded_weapons()) {
        if (sizeof(types)) {
            t = w->query_type();
            if (member_array(t, types) == -1) continue;
            if (member_array(w->query_hands(), __Weapons[t]) == -1) continue;
        }
        if (w->query_wc() <= wc) continue;
        wc = w->query_wc();
        weapon = w;
    }

    return weapon;
}