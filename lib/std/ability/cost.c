// @this_object /std/ability.c

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