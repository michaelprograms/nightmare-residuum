nosave mapping __Reqs;

mapping query_ability_requirements () {
    return __Reqs;
}
void set_ability_requirements (mapping reqs) {
    /* Data format:
    "anyone|class|species|NPC": ([
        "level" : 1,
        "stats" : ([ "strength": 1, ]),
    ]),
    */
    __Reqs = reqs;
}
int verify_ability_requirements (object source) {
    if (!source || !source->is_living()) {
        return 0;
    } else if (source->query_immortal()) {
        return 1;
    }

    // no requirements exist
    if (!sizeof(__Reqs)) {
        return 1;
    }

    foreach (string key,mapping value in __Reqs) {
        if (key == "NPC") {
            if (!source->is_npc()) {
                continue;
            }
        } else if (source->query_class() != key && source->query_species() != key && key != "anyone") {
            continue;
        }
        if (intp(value["level"])) {
            if (source->query_level() < value["level"]) {
                return 0;
            }
        }
        if (mapp(value["stats"])) {
            foreach (string stat,int num in value["stats"]) {
                if (source->query_stat(stat) < num) {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
}