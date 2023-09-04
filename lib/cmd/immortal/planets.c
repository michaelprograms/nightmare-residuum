inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("planets");
    set_help_text("The planets command is used to view the entire list of planets available.");
}

void command (string input, mapping flags) {
    mixed *planets;
    string *items = ({ });
    int n;

    if (n = sizeof(planets = D_PLANET->query_planets())) {
        foreach (mixed *planet in planets) {
            items += ({ planet... });
        }
    }

    border(([
        "title": "PLANETS",
        "subtitle": mud_name(),
        "body": ([
            "header": ({ "Name", "Seed", "Size", }),
            "items": items,
            "columns": 3,
        ]),
        "footer": ([
            "items": ({ n + " planet" + (n > 1 ? "s" : "") }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
}