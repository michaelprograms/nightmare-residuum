// @this_object /std/ability.c

/**
 * Determine how much this ability costs to cast.
 *
 * @returns a mapping of vitals
 */
mapping query_cost () {
    string vitalType;
    mapping cost = ([
        "sp": 0,
        "mp": 0,
    ]);

    foreach (string key,int value in this_object()->query_powers()) {
        switch (key) {
        case "psionic":
            vitalType = "mp";
            break;
        case "ranged": case "brawl": default:
            vitalType = "sp";
            break;
        }
        cost[vitalType] += value;
    }
    return cost;
}

/**
 * Verifies the source has enough vitals to cast this ability.
 *
 * @param {STD_LIVING} source
 * @param cost from query_cost function
 * @returns 0 or 1 for being able to cast
 */
int verify_cost (object source, mapping cost) {
    // check source vitals
    if (cost["sp"] > 0 && source->query_sp() < cost["sp"]) {
        message("action", "You are too drained to " + this_object()->query_name() + ".", source);
        return 0;
    }
    if (cost["mp"] > 0 && source->query_mp() < cost["mp"]) {
        message("action", "You are too tired to " + this_object()->query_name() + ".", source);
        return 0;
    }
    return 1;
}

/**
 * Applies the cost to the source's vitals.
 *
 * @param {STD_LIVING} source
 * @param cost from query_cost function
 */
void apply_cost (object source, mapping cost) {
    // update source vitals
    if (cost["sp"] > 0) {
        source->add_sp(-(cost["sp"]*3/4 + (random(cost["sp"]/4) + 1)));
    }
    if (cost["mp"]) {
        source->add_mp(-(cost["mp"]*3/4 + (random(cost["mp"]/4) + 1)));
    }
}