void command (string input, mapping flags) {
    string *border, *body = ({ });
    mapping a, footer;
    int t = time(), currentMonth, currentDay;

    a = D_ASTRONOMY->query_astronomy_from_room(environment(this_character()));

    if (a) {
        currentMonth = D_ASTRONOMY->query_month(t, a);
        currentDay = D_ASTRONOMY->query_week(t, a) * a["DAYS_PER_WEEK"] + D_ASTRONOMY->query_day(t, a);

        for (int i = 0; i < sizeof(a["MONTH_NAMES"]); i += 4) {
            string *rowHeader = ({ }), *rowItems = ({ });

            for (int j = 0; j < 4; j ++) {
                if (i+j >= sizeof(a["MONTH_NAMES"])) break;
                else if (i+j == currentMonth) rowHeader += ({ "  %^CYAN%^" + a["MONTH_NAMES"][i+j] + "%^RESET%^" });
                else rowHeader += ({ "  "+a["MONTH_NAMES"][i+j] });
            }
            for (int k = 0; k < 4; k ++) {
                for (int j = 0; j < 4; j ++) {
                    string row = "";
                    for (int l = 1; l < 6; l ++) {
                        int n = k*5+l;
                        if (i+j >= sizeof(a["MONTH_NAMES"])) continue;
                        if (i+j == currentMonth && n-1 == currentDay) {
                            row += (n < 10 ? " " : "") + "%^CYAN%^UNDERLINE%^"+n+"%^RESET%^ ";
                        } else {
                            row += (n < 10 ? " " : "") + n + " ";
                        }
                    }
                    rowItems += ({ row });
                }
            }
            body += ({
                ([
                    "header": rowHeader,
                    "items": rowItems,
                    "columns": 4,
                    "align": "center",
                ]),
            });
        }
        footer = ([
            "items": ({
                "Today is " + D_ASTRONOMY->query_localdate(a, t),
            }),
            "columns": 1,
            "align": "center",
        ]);
    }

    border = format_border(([
        "title": "CALENDAR",
        "body": a ? body : ({
            ([
                "items": ({
                    "No local astronomy found",
                }),
                "columns": 1,
                "align": "center",
            ]),
        }),
        "footer": footer,
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}