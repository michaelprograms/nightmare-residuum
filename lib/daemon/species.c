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
        string species, bodytype;

        if (sscanf(line, "%s:%s", species, bodytype) != 2) continue;
        if (!mapp(__Species[species])) __Species[species] = ([]);

        __Species[species]["bodytype"] = bodytype;
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
    }

    return body;
}

/* -----  ----- */

void create () {
    set_no_clean(clean_later());
    load_bodytypes();
    load_species();
}
