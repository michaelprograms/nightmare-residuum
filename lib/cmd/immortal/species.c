inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("species (type)");
    set_help_text("The species command can be used to view the supported bodytypes and species. Supplying an optional type will filter.");
}

void command (string input, mapping flags) {
    string subtitle;
    mapping *items = ({ });

    if (input) {
        if (member_array(input, keys(D_SPECIES->query_species())) > -1) {
            mapping m = D_SPECIES->query_species()[input];
            subtitle = input + " species";
            items += ({
                ([
                    "header": capitalize(input),
                    "items": map(keys(m), (:
                        capitalize($1) + ": " + $(m)[$1]
                    :)),
                    "columns": 3,
                ])
            });
        }
        if (member_array(input, keys(D_SPECIES->query_bodies())) > -1) {
            mapping m = D_SPECIES->query_bodies()[input];
            string *limbs = keys(m);
            subtitle = input + " bodytype";

            for (int i = 0; i < sizeof(limbs); i ++) {
                string limb = limbs[i];
                limbs[i] = capitalize(limb) + ": " + implode(map(keys(m[limb]), (: $(m)[$(limb)][$1] + " " + $1 :)), ", ");
            }
            items += ({
                ([
                    "header": capitalize(input),
                    "items": limbs,
                    "columns": 1,
                ])
            });
        }
    }

    if (!sizeof(items)) {
        items += ({
            ([
                "header": ({ "Bodytypes" }),
                "items": keys(D_SPECIES->query_bodies()),
                "columns": 3,
            ])
        });
        items += ({
            ([
                "header": ({ "Species" }),
                "items": keys(D_SPECIES->query_species()),
                "columns": 3,
            ])
        });
    }

    border(([
        "title": "SPECIES",
        "subtitle": subtitle,
        "body": items,
    ]));
}