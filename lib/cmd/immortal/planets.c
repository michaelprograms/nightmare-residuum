inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("planets");
    set_help_text("The planets command is used to view the entire list of planets available.");
}

void command (string input, mapping flags) {
    mixed *results;
    mixed *body = ({ });
    mapping item = ([ ]);
    int n;

    if (input) {
        if (n = sizeof(results = D_PLANET->query_planet_chunks(input))) {
            foreach (mixed *chunk in results) {
                item = ([
                    "header": ({ chunk[0] }),
                    "items": explode(chunk[4], ","),
                    "columns": 1,
                ]);
                body += ({ item });
            }
        }
    } else {
        if (n = sizeof(results = D_PLANET->query_planets())) {
            item = ([
                "header": ({ "Name", "Seed", "Size", }),
                "items": ({ }),
                "columns": 3,
            ]);
            foreach (mixed *planet in results) {
                item["items"] += ({ planet... });
            }
            body += ({ item });
        }
    }

    border(([
        "title": "PLANETS",
        "subtitle": mud_name(),
        "body": body,
        "footer": ([
            "items": ({ n + " planet" + (n > 1 ? "s" : "") }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
}