void command (string input, mapping flags) {
    string *border;
    mapping *items = ({ });

    foreach(string key,mapping a in D_ASTRONOMY->query_astronomy()) {
        items += ({
            ([
                "header": key,
                "items": ({
                    "TIME LEGEND",
                    "",
                    "",
                    a["SECONDS_PER_MINUTE"] + " secs per min",
                    a["MINUTES_PER_HOUR"] + " mins per hour",
                    a["HOURS_PER_DAY"] + " hours per day",
                    a["DAYS_PER_WEEK"] + " days per week",
                    a["WEEKS_PER_MONTH"] + " weeks per month",
                    a["MONTHS_PER_YEAR"] + " months per year",
                    "Total days: " + a["TOTAL_DAYS"],
                    "Shortest day: " + a["SHORTEST_DAY"],
                    "Longest day: " + a["LONGEST_DAY"],
                    "",
                    "",
                    "",
                    "PHASE",
                    "",
                    "",
                    D_ASTRONOMY->query_localdate(key),
                    "Time: " + D_ASTRONOMY->query_localtime(key),
                    "Day phase: " + a["DAY_PHASE"],
                    "Next phase: " + (a["NEXT_PHASE"] - time()) + " secs",
                    "",
                    "",
                    "",
                    "",
                    "",
                    "ALMANAC",
                    "",
                    "",
                    "Dawn: " + a["ALMANAC"]["dawn"][0] + ":" + D_ASTRONOMY->format_minute(a["ALMANAC"]["dawn"][1]),
                    "Day: " + a["ALMANAC"]["day"][0] + ":" + D_ASTRONOMY->format_minute(a["ALMANAC"]["day"][1]),
                    "Dusk: " + a["ALMANAC"]["dusk"][0] + ":" + D_ASTRONOMY->format_minute(a["ALMANAC"]["dusk"][1]),
                    "Night: " + a["ALMANAC"]["night"][0] + ":" + D_ASTRONOMY->format_minute(a["ALMANAC"]["night"][1]),
                    "",
                    "",
                    "",
                    "",
                    "",
                    "MOONS",
                    "",
                    "",
                }) + map(keys(a["MOONS"]), (: capitalize($1)+" ["+$(a)["MOONS"][$1]["phase"]+"], "+$(a)["MOONS"][$1]["orbit"]+" days" :)),
                "columns": 3,
            ])
        });
    }

    border = format_border(([
        "title": "ASTRONOMY",
        "body": items,
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}