inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("planets");
    set_help_text("The planets command is used to view the entire list of planets available.");
}

void command (string input, mapping flags) {
    mixed *results, *subresults;
    mapping planet;
    mixed *body = ({ });
    mapping item = ([ ]);
    int n;

    item = ([
        "header": ({ "Name", "Size", "Overrides", }),
        "items": ({ }),
        "columns": 3,
    ]);

    if (input) {
        planet = D_PLANET->query_planet(input);

        if (planet["name"]) {
            item["items"] += ({ planet["name"], planet["size"], sizeof(planet["overrides"]) });
        }
    } else {
        results = filter(get_dir("/save/planet/", -1), (: $1[1] == -2 :));
        if (n = sizeof(results)) {
            foreach (string dir in results) {
                subresults = get_dir("/save/planet/" + dir[0] + "/*.o");
                foreach (mixed *file in subresults) {
                    planet = D_PLANET->query_planet(file[0..<3]);
                    item["items"] += ({ planet["name"], planet["size"], sizeof(planet["overrides"]) });
                }
            }
        }
    }
    body += ({ item });



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