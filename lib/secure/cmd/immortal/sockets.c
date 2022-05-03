void command (string input, mapping flags) {
    string *border, *sockets = ({}), *netStats = ({ });

    foreach (mixed *s in socket_status()) {
        sockets += ({ s[0], s[1], s[2], s[3], });
    }

    foreach (string key, int value in filter_array(network_stats(), (: strsrch($1, "socket") > -1 :))) {
        key = replace_string(key, " sockets", "");
        netStats += ({ sprintf("%20s", key) + " : " + value });
    }
    netStats = sort_array(netStats, (: strcmp(ltrim($1), ltrim($2)) :));

    border = format_border(([
        "title": "SOCKETS",
        "subtitle": mud_name(),
        "body": ([
            "header": ({ "Socket", "State", "Mode", "Local Address", }),
            "items": sockets,
            "columns": 4,
        ]),
        "footer": ([
            "items": netStats,
            "columns": 2,
        ]),
    ]));
    foreach (string line in border) {
        message("system", line + "\n", this_character());
    }
}