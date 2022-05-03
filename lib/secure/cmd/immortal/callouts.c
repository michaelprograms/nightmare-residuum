void command (string input, mapping flags) {
    mixed *callouts;
    string *border, *items = ({ });

    if (sizeof(callouts = call_out_info())) {
        foreach (mixed *callout in callouts) {
            callout[1] = replace_string(callout[1], "\n", "\\n");
            items += ({ callout... });
        }
    }

    border = format_border(([
        "title": "SOCKETS",
        "subtitle": mud_name(),
        "body": ([
            "header": ({ "Object", "Function", "Delay", }),
            "items": items,
            "columns": 3,
        ]),
        "footer": ([
            "items": ({ sizeof(items) + " callouts" }),
            "columns": 1,
            "align": "center",
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}