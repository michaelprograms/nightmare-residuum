void command (string input, mapping flags) {
    string *border, *sockets = ({}), *netStats = ({ });

    sockets = map(socket_status(), (: sprintf("%2d    %-9s    %-8s    %-20s", $1[0], $1[1], $1[2], $1[3]) :));

    foreach (string key, int value in filter_array(network_stats(), (:strsrch($1, "socket") > -1:))) {
        key = replace_string(key, " sockets", "");
        netStats += ({ sprintf("%20s", key) + " : " + value });
    }
    netStats = sort_array(netStats, -1);

    border = format_border(([
        "title": "SOCKETS",
        "subtitle": mud_name(),
        "body": ([
            "header": "Fd    State        Mode        Local Address",
            "items": sockets,
            "columns": 1,
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