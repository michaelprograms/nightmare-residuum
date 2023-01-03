#define CFG_BODYTYPES "/etc/bodytypes.cfg"
#define CFG_SPECIES "/etc/species.cfg"

inherit M_CLEAN;

nosave private mapping __BodyTypes = ([ ]);
nosave private mapping __Species = ([ ]);

mapping query_bodytypes () { return __BodyTypes; }
mapping query_species () { return __Species; }

/* ----- load configs ----- */

private void load_bodytypes () {
    string file = read_file(CFG_BODYTYPES), *lines;

    lines = filter_array(explode(file, "\n"), (: $1 && $1 != "" && $1[0] != '#' :));

    foreach (string line in lines) {
        string bodytype, limb, attach, limbtype;
        int percent;

        if (sscanf(line, "%s:%s,%s,%s,%d", bodytype, limb, attach, limbtype, percent) != 5) continue;
        if (percent > 100) percent = 100;
        else if (percent < 1) percent = 1;

        if (!mapp(__BodyTypes[bodytype])) __BodyTypes[bodytype] = ([]);
        if (!mapp(__BodyTypes[bodytype][limb])) __BodyTypes[bodytype][limb] = ([]);
        if (attach != "0") __BodyTypes[bodytype][limb]["attach"] = attach;
        if (limbtype != "0") __BodyTypes[bodytype][limb]["limbtype"] = limbtype;
        __BodyTypes[bodytype][limb]["percent"] = percent;
    }
}

private void load_species () {
    string file = read_file(CFG_SPECIES), *lines;

    lines = filter_array(explode(file, "\n"), (: $1 && $1 != "" && $1[0] != '#' :));

    foreach (string line in lines) {
        string species = 0, bodytype = 0, stats = 0;

        if (
            sscanf(line, "%s:%s:%s", species, bodytype, stats) != 3 &&
            sscanf(line, "%s:%s", species, bodytype) != 2
        ) continue;

        if (!mapp(__Species[species])) __Species[species] = ([ ]);

        __Species[species]["bodytype"] = bodytype;
        if (stats) {
            string *s = explode(stats, ",");

            if (sizeof(s) != 7) continue;

            __Species[species]["strength"] = to_int(s[0]);
            __Species[species]["perception"] = to_int(s[1]);
            __Species[species]["endurance"] = to_int(s[2]);
            __Species[species]["charisma"] = to_int(s[3]);
            __Species[species]["intelligence"] = to_int(s[4]);
            __Species[species]["agility"] = to_int(s[5]);
            __Species[species]["luck"] = to_int(s[6]);
        }
    }
}

/* -----  ----- */

int query_adjust_stat (string s, string stat) {
    return __Species[s] && __Species[s][stat] ? __Species[s][stat] : 0;
}
void setup_stats (object ob) {
    int adjust;
    string s = ob->query_species();

    foreach (string stat in ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck" })) {
        adjust = query_adjust_stat(s, stat);
        ob->set_stat(stat, 5 + (adjust * 2));
    }
}

/* -----  ----- */

mapping setup_body (object ob) {
    mapping body = ([]);
    mapping limbs;
    string species, bodytype;

    if (!ob || !(species = ob->query_species()) || !mapp(__Species[species])) return 0;

    bodytype = __Species[species]["bodytype"];
    limbs = __BodyTypes[bodytype];

    foreach (string limb in keys(limbs)) {
        body[limb] = ([]);
        if (limbs[limb]["limbtype"]) body[limb]["type"] = limbs[limb]["limbtype"];
        if (limbs[limb]["attach"]) body[limb]["attach"] = limbs[limb]["attach"];
        body[limb]["pct"] = limbs[limb]["percent"];
        body[limb]["maxdamage"] = 1 + ob->query_max_hp() * body[limb]["pct"] / 100;
        body[limb]["damage"] = 0;
        body[limb]["status"] = 0;
    }

    return body;
}

/* -----  ----- */

void create () {
    set_no_clean(1);
    load_bodytypes();
    load_species();
}
