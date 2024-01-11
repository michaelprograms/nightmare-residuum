#define CFG_bodyS "/etc/bodys.cfg"
#define CFG_SPECIES "/etc/species.cfg"

inherit M_CLEAN;

/* Body Example:
    "body name":      ([
        "limb name": ([
            "type": "FATAL|WIELD",
            "attach": limb,
            "percent": 1..100,
        ]),
    ]),
*/
nosave private mapping __Bodies = ([
    /* Character Bodies */
    "biped": ([
        "head": ([ "type": "FATAL", "percent": 50 ]),
        "torso": ([ "type": "FATAL", "percent": 100 ]),
        "left arm": ([ "attach": "left hand", "percent": 30 ]),
        "left hand": ([ "type": "WIELD", "percent": 25 ]),
        "right arm": ([ "attach": "right hand", "percent": 30 ]),
        "right hand": ([ "type": "WIELD", "percent": 25 ]),
        "left leg": ([ "attach": "left foot", "percent": 35 ]),
        "left foot": ([ "percent": 25 ]),
        "right leg": ([ "attach": "right foot", "percent": 35 ]),
        "right foot": ([ "percent": 25 ]),
    ]),
    /* NPC Bodies */
    "fowl": ([
        "head": ([ "attach": "beak", "type": "FATAL", "percent": 50 ]),
        "beak": ([ "type": "WIELD", "percent": 20 ]),
        "torso": ([ "type": "FATAL", "percent": 50 ]),
        "left wing": ([ "percent": 30 ]),
        "right wing": ([ "percent": 30 ]),
        "left leg": ([ "attach": "left claw", "percent": 25 ]),
        "left claw": ([ "type": "WIELD", "percent": 20 ]),
        "right leg": ([ "attach": "right claw", "percent": 25 ]),
        "right claw": ([ "type": "WIELD", "percent": 20 ]),
        "tail": ([ "percent": 25 ]),
    ]),
    "gastropod": ([
        "head": ([ "type": "FATAL", "percent": 25 ]),
        "left eye stalk": ([ "percent": 20 ]),
        "right eye stalk": ([ "percent": 20 ]),
        "left tentacle": ([ "percent": 20 ]),
        "right tentacle": ([ "percent": 20 ]),
        "tail": ([ "percent": 50 ]),
        "foot": ([ "percent": 25 ]),
        "mantle": ([ "percent": 50 ]),
    ]),
    "gastropod-shelled": ([
        "head": ([ "type": "FATAL", "percent": 25 ]),
        "left eye stalk": ([ "percent": 20 ]),
        "right eye stalk": ([ "percent": 20 ]),
        "left tentacle": ([ "percent": 20 ]),
        "right tentacle": ([ "percent": 20 ]),
        "tail": ([ "percent": 50 ]),
        "foot": ([ "percent": 25 ]),
        "shell": ([ "percent": 100 ]),
    ]),
    "hexapod": ([
        "head": ([ "type": "FATAL", "percent": 50 ]),
        "thorax": ([ "type": "FATAL", "percent": 100 ]),
        "abdomen": ([ "type": "FATAL", "percent": 50 ]),
        "left fore leg": ([ "type": "WIELD", "percent": 25 ]),
        "right fore leg": ([ "type": "WIELD", "percent": 25 ]),
        "left mid leg": ([ "type": "WIELD", "percent": 25 ]),
        "right mid leg": ([ "type": "WIELD", "percent": 25 ]),
        "left hind leg": ([ "type": "WIELD", "percent": 25 ]),
        "right hind leg": ([ "type": "WIELD", "percent": 25 ]),
    ]),
    "hexapod-winged": ([
        "head": ([ "type": "FATAL", "percent": 50 ]),
        "abdomen": ([ "type": "FATAL", "percent": 50 ]),
        "thorax": ([ "type": "FATAL", "percent": 100 ]),
        "left fore leg": ([ "type": "WIELD", "percent": 25 ]),
        "right fore leg": ([ "type": "WIELD", "percent": 25 ]),
        "left mid leg": ([ "type": "WIELD", "percent": 25 ]),
        "right mid leg": ([ "type": "WIELD", "percent": 25 ]),
        "left hind leg": ([ "type": "WIELD", "percent": 25 ]),
        "right hind leg": ([ "type": "WIELD", "percent": 25 ]),
        "left fore wing": ([ "percent": 20 ]),
        "right fore wing": ([ "percent": 20 ]),
        "left hind wing": ([ "percent": 20 ]),
        "right hind wing": ([ "percent": 20 ]),
    ]),
    "quadruped": ([
        "head": ([ "type": "FATAL", "percent": 50 ]),
        "torso": ([ "type": "FATAL", "percent": 100 ]),
        "left fore leg": ([ "attach": "left fore paw", "percent": 30 ]),
        "left fore paw": ([ "type": "WIELD", "percent": 25 ]),
        "right fore leg": ([ "attach": "right fore paw", "percent": 30 ]),
        "right fore paw": ([ "type": "WIELD", "percent": 25 ]),
        "left rear leg": ([ "attach": "left rear paw", "percent": 35 ]),
        "left rear paw": ([ "percent": 25 ]),
        "right rear leg": ([ "attach": "right rear paw", "percent": 35 ]),
        "right rear paw": ([ "percent": 25 ]),
    ]),
    "quadruped-robot": ([
        "torso": ([ "type": "FATAL", "percent": 100 ]),
        "left fore leg": ([ "attach": "left fore paw", "percent": 30 ]),
        "left fore paw": ([ "type": "WIELD", "percent": 25 ]),
        "right fore leg": ([ "attach": "right fore paw", "percent": 30 ]),
        "right fore paw": ([ "type": "WIELD", "percent": 25 ]),
        "left rear leg": ([ "attach": "left rear paw", "percent": 35 ]),
        "left rear paw": ([ "percent": 25 ]),
        "right rear leg": ([ "attach": "right rear paw", "percent": 35 ]),
        "right rear paw": ([ "percent": 25 ]),
    ]),
    "quadruped-tail": ([
        "head": ([ "type": "FATAL", "percent": 50 ]),
        "torso": ([ "type": "FATAL", "percent": 100 ]),
        "left fore leg": ([ "attach": "left fore paw", "percent": 30 ]),
        "left fore paw": ([ "type": "WIELD", "percent": 25 ]),
        "right fore leg": ([ "attach": "right fore paw", "percent": 30 ]),
        "right fore paw": ([ "type": "WIELD", "percent": 25 ]),
        "left rear leg": ([ "attach": "left rear paw", "percent": 35 ]),
        "left rear paw": ([ "percent": 25 ]),
        "right rear leg": ([ "attach": "right rear paw", "percent": 35 ]),
        "right rear paw": ([ "percent": 25 ]),
        "tail": ([ "percent": 20 ]),
    ]),
]);
/* Species Example:
    "species name":      ([
        "body": body,           // required
        "attributes": ([ ]),    // optional
        "stats": ([
            "strength": n,      // optional
            "perception": n,    // optional
            "endurance": n,     // optional
            "charisma": n,      // optional
            "intelligence": n,  // optional
            "agility": n,       // optional
            "luck": n,          // optional
        ]),
    ]),
*/
nosave private mapping __Species = ([
    /* Character Species */
    "human":        ([
        "body": "biped",
        "stats": ([
            "strength": 2,
            "perception": 1,
            "endurance": 1,
            "charisma": 1,
            "intelligence": 2,
            "agility": 2,
            "luck": 1,
        ]),
        "vitals": ([
            "hp": 1,
            "sp": 1,
            "mp": 1,
        ]),
        "attributes": ([
            "build": ({ "athletic", "lean", "muscular", "slender", "stout", }),
            "complexion": ({ "fair", "medium", "dark", }),
            "eye": ({ "amber", "black", "blue", "brown", "green", "hazel", }),
            "hair": ({ "auburn", "bald", "black", "blonde", "brown", "gray", }),
            "height": ([ "min": 160, "max": 180 ]),
        ]),
    ]),
    "zotarian":     ([
        "body": "biped",
        "stats": ([
            "strength": 1,
            "perception": 2,
            "endurance": 1,
            "charisma": 2,
            "intelligence": 2,
            "agility": 1,
            "luck": 1,
        ]),
        "vitals": ([
            "hp": 0,
            "sp": 0,
            "mp": 3,
        ]),
    ]),
    /* NPC Species */
    "ant":          ([ "body": "hexapod" ]),
    "bee":          ([ "body": "hexapod-winged" ]),
    "beetle":       ([ "body": "hexapod" ]),
    "crow":         ([ "body": "fowl" ]),
    "cat":          ([ "body": "quadruped-tail" ]),
    "dog":          ([ "body": "quadruped-tail" ]),
    "fly":          ([ "body": "hexapod-winged" ]),
    "hamster":      ([ "body": "quadruped" ]),
    "pigeon":       ([ "body": "fowl" ]),
    "sparrow":      ([ "body": "fowl" ]),
    "rat":          ([ "body": "quadruped-tail" ]),
    "slug":         ([ "body": "gastropod" ]),
    "snail":        ([ "body": "gastropod-shelled" ]),
    "squirrel":     ([ "body": "quadruped-tail" ]),
    "custodian":    ([ "body": "quadruped-robot" ]),
    "termite":      ([ "body": "hexapod" ]),
    "vulture":      ([ "body": "fowl" ]),
    "wasp":         ([ "body": "hexapod-winged" ]),
]);

mapping query_bodies () { return __Bodies; }
mapping query_species () { return __Species; }

/* ----- species adjustments ----- */

int query_adjust_hp (string s) {
    if (__Species[s] && __Species[s]["vitals"]) {
        return __Species[s]["vitals"]["hp"];
    } else {
        return 0;
    }
}
int query_adjust_sp (string s) {
    if (__Species[s] && __Species[s]["vitals"]) {
        return __Species[s]["vitals"]["sp"];
    } else {
        return 0;
    }
}
int query_adjust_mp (string s) {
    if (__Species[s] && __Species[s]["vitals"]) {
        return __Species[s]["vitals"]["mp"];
    } else {
        return 0;
    }
}

int query_adjust_stat (string s, string stat) {
    if (__Species[s] && __Species[s]["stats"]) {
        return __Species[s]["stats"][stat];
    } else {
        return 0;
    }
}

/* ----- setup ----- */

// initial stats for a STD_CHARACTER
void setup_stats (object ob) {
    int adjust;
    string s = ob->query_species();

    foreach (string stat in ({ "strength", "perception", "endurance", "charisma", "intelligence", "agility", "luck" })) {
        adjust = query_adjust_stat(s, stat);
        ob->set_stat(stat, 5 + (adjust * 2));
    }
}

// new body for a STD_LIVING
mapping setup_body (object ob) {
    mapping newBody = ([ ]);
    mapping limbs;
    string species, body;

    if (!ob || !(species = ob->query_species()) || !mapp(__Species[species])) return 0;

    body = __Species[species]["body"];
    limbs = __Bodies[body];

    foreach (string limb in keys(limbs)) {
        newBody[limb] = ([ ]);
        if (limbs[limb]["type"]) {
            newBody[limb]["type"] = limbs[limb]["type"];
        }
        if (limbs[limb]["attach"]) {
            newBody[limb]["attach"] = limbs[limb]["attach"];
        }
        newBody[limb]["pct"] = limbs[limb]["percent"];
        newBody[limb]["maxdamage"] = 1 + ob->query_max_hp() * newBody[limb]["pct"] / 100;
        newBody[limb]["damage"] = 0;
        newBody[limb]["status"] = 0;
    }

    return newBody;
}

/* ----- applies ----- */

void create () {
    set_no_clean(1);
}
