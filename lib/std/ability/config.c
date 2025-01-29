nosave private string __Type;
nosave private mapping __Powers = ([ ]);

void set_type (string type) {
    if (member_array(type, ({ "attack", "heal", "utility", })) == -1) {
        error("Bad argument 1 to ability/config->set_type");
    }
    __Type = type;
}
string query_type () {
    return __Type;
}

mapping query_powers () {
    return __Powers;
}
void set_powers (mapping powers) {
    if (undefinedp(powers) || !mapp(powers)) {
        error("Bad argument 1 to ability/config->set_powers");
    }
    __Powers = powers;
}
int query_total_skill_power () {
    int total = 0;
    foreach (string key,int value in __Powers) {
        total += value;
    }
    return total;
}