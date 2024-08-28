inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("planets");
    set_help_text("The planets command is used to view the entire list of planets available.");
}

string format_factor (float f) {
    return undefinedp(f) ? "1.00" : sprintf("%.2f", f);
}

void command (string input, mapping flags) {
    mapping planet;
    mixed *body = ({ });
    mapping item = ([ ]);
    int n;

    item = ([
        "header": ({ "Name", "Size", "Overrides", "Height", "Humidity", "Heat", }),
        "items": ({ }),
        "columns": 6,
    ]);

    if (input) {
        planet = D_PLANET->query_planet(input);

        if (planet["name"]) {
            item["items"] += ({
                planet["name"],
                planet["size"],
                sizeof(planet["overrides"]),
                format_factor(planet["heightFactor"]),
                format_factor(planet["humidityFactor"]),
                format_factor(planet["heatFactor"]),
            });
        }
    } else {
        foreach (string p in D_PLANET->query_all_planets()) {
            planet = D_PLANET->query_planet(p);
            item["items"] += ({
                planet["name"],
                planet["size"],
                sizeof(planet["overrides"]),
                format_factor(planet["heightFactor"]),
                format_factor(planet["humidityFactor"]),
                format_factor(planet["heatFactor"]),
            });
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