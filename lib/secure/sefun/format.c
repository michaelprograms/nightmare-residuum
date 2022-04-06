private string query_account_setting (string setting) {
    object po = previous_object(-1)[<1], account;

    if (strsrch(D_TEST, base_name(po)) == 0) po = previous_object(-1)[<2];

    if (!(account = po->query_account())) return 0;

    return account->query_setting(setting);
}

varargs string format_header_bar (string title, string optional) {
    int width, n = 0, ansi;
    string bar;

    if (!stringp(title)) error("Bad argument 1 to format->format_header_bar");

    if (query_account_setting("screenreader") != "on") {
        ansi = query_account_setting("ansi") == "on";
        width = to_int(query_account_setting("width"));

        bar = (ansi?"%^RESET%^CYAN%^":"") + "==";
        bar += (ansi?"%^RESET%^":"") + "/ " + (ansi?"%^RESET%^BOLD%^":"") + title;
        n += 4 + strlen(title);

        if (optional) {
            bar += ":" + (ansi?"%^RESET%^":"") + " " + optional;
            n += 2 + strlen(optional);
        } else {
            bar += (ansi?"%^RESET%^":"");
        }
        bar += " \\" + (ansi?"%^CYAN%^":"") + "=";
        n += 3;

        for (int i = n; i < width; i ++) bar += "=";
        bar += (ansi?"%^RESET%^":"");
    } else {
        bar = title + (optional ? ": " + optional : "");
    }
    return bar;
}
string format_divider_bar () {
    int width, ansi;
    string bar = "";

    if (query_account_setting("screenreader") != "on") {
        ansi = query_account_setting("ansi") == "on";
        width = to_int(query_account_setting("width"));

        bar = (ansi?"%^RESET%^BLUE%^":"") + "-------------------------";
        for (int i = 25; i < width; i ++) bar += "-";
        bar += (ansi?"%^RESET%^":"");
    }
    return bar;
}
string format_footer_bar () {
    int width, ansi;
    string bar = "";

    if (query_account_setting("screenreader") != "on") {
        ansi = query_account_setting("ansi") == "on";
        width = to_int(query_account_setting("width"));

        bar = (ansi?"%^RESET%^CYAN%^":"") + "=========================";
        for (int i = 25; i < width; i ++) bar += "=";
        bar += (ansi?"%^RESET%^":"");
    }
    return bar;
}

varargs string format_page (string *items, int columns, int pad) {
    int width, i, j, n;
    string *rows = ({});

    if (!arrayp(items) || !sizeof(items)) error("Bad argument 1 to format->format_page");

    if (!columns) columns = 2;
    width = to_int(query_account_setting("width"));
    width = (width - pad*2) / columns;
    n = sizeof(items);

    for (i = 0; i < n; i += columns) {
        string row = "";
        for (j = 0; j < columns; j ++) {
            if (i + j >= n) {
                // pad remainder of line
                row += sprintf("%' '"+sprintf("%d", (columns-j)*width)+"s", " ");
                break;
            }
            row += sprintf("%-"+sprintf("%d", width)+"s", items[i + j]); // @TODO for longer strings
        }
        rows += ({ row });
    }
    return implode(rows, "\n");
}

string format_exit_brief (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_brief");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "north": result += ({ "n" }); break;
            case "northeast": result += ({ "ne" }); break;
            case "east": result += ({ "e" }); break;
            case "southeast": result += ({ "se" }); break;
            case "south": result += ({ "s" }); break;
            case "southwest": result += ({ "sw" }); break;
            case "west": result += ({ "w" }); break;
            case "northwest": result += ({ "nw" }); break;
            case "up": result += ({ "u" }); break;
            case "down": result += ({ "d" }); break;
            case "enter": result += ({ "ent" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}
string format_exit_verbose (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_verbose");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "n": result += ({ "north" }); break;
            case "ne": result += ({ "northeast" }); break;
            case "e": result += ({ "east" }); break;
            case "se": result += ({ "southeast" }); break;
            case "s": result += ({ "south" }); break;
            case "sw": result += ({ "southwest" }); break;
            case "w": result += ({ "west" }); break;
            case "nw": result += ({ "northwest" }); break;
            case "u": result += ({ "up" }); break;
            case "d": result += ({ "down" }); break;
            case "ent": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}
string format_exit_reverse (string dir) {
    string *result = ({});
    if (!stringp(dir)) error("Bad argument 1 to format->format_exit_reverse");
    foreach (string part in explode(dir, " ")) {
        switch (part) {
            case "north": result += ({ "south" }); break;
            case "northeast": result += ({ "southwest" }); break;
            case "east": result += ({ "west" }); break;
            case "southeast": result += ({ "northwest" }); break;
            case "south": result += ({ "north" }); break;
            case "southwest": result += ({ "northeast" }); break;
            case "west": result += ({ "east" }); break;
            case "northwest": result += ({ "southeast" }); break;
            case "up": result += ({ "down" }); break;
            case "down": result += ({ "up" }); break;
            case "enter": result += ({ "out" }); break;
            case "out": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}

string format_stat_brief (string stat) {
    string result = "";
    if (!stringp(stat)) error("Bad argument 1 to format->format_stat_brief");
    switch (stat) {
        case "strength": case "str": result = "str"; break;
        case "perception": case "per": result = "per"; break;
        case "endurance": case "end": result = "end"; break;
        case "charisma": case "cha": result = "cha"; break;
        case "intelligence": case "int": result = "int"; break;
        case "agility": case "agi": result = "agi"; break;
        case "luck": case "lck": result = "lck"; break;
    }
    return result;
}
string format_stat_verbose (string stat) {
    string result = "";
    if (!stringp(stat)) error("Bad argument 1 to format->format_stat_verbose");
    switch (stat) {
        case "str": case "strength": result = "strength"; break;
        case "per": case "perception": result = "perception"; break;
        case "end": case "endurance": result = "endurance"; break;
        case "cha": case "charisma": result = "charisma"; break;
        case "int": case "intelligence": result = "intelligence"; break;
        case "agi": case "agility": result = "agility"; break;
        case "lck": case "luck": result = "luck"; break;
    }
    return result;
}

string format_integer (int num) {
    string *digits, result = "";
    int neg, s;

    if (!intp(num)) error("Bad argument 1 to format->format_integer");

    neg = (num < 0);
    num = abs(num);
    digits = explode(""+to_int(num), "");
    s = sizeof(digits) - 1;

    for(int i = s; i >= 0; i --) {
        result = digits[i] + (i != s && (s-i)%3 == 0 ? "," : "") + result;
    }

    return (neg ? "-" : "") + result;
}


// create a unicode border around action output
//
// mapping data = ([
// /*
//     "title": "TITLE",
//     "subtitle": "Subtitle",
//     "header": ([
//         "items": ({}),
//         "columns": 2,
//         "align": "left|center",
//     ]),
//     "body": ([
//         "items": ({}),
//         "columns": 2,
//         "align": "left|center",
//     ]),
//     "footer": ([
//         "items": ({}),
//         "columns": 2,
//         "align": "left|center",
//     ]),
// */
// ]);
string *format_border (mapping data) {
    string *lines = ({ }), line;

    int fTitle = !undefinedp(data["title"]);
    int fSubtitle = !undefinedp(data["subtitle"]);
    int fHeader = !undefinedp(data["header"]);
    int fBody = !undefinedp(data["body"]);
    int fFooter = !undefinedp(data["footer"]);

    // @TODO
    // string encoding;
    // encoding = query_encoding();

    if (query_account_setting("screenreader") != "on") {
        int width, ansi, n;

        ansi = query_account_setting("ansi") == "on";
        width = to_int(query_account_setting("width"));
        n = 0;

        if (fTitle) {
            // Title Line 1
            line = (ansi?"%^RESET%^CYAN%^":"") + "   \u250c" + sprintf("%'\u2500'"+sprintf("%d", 2 + strlen(data["title"]) + (fSubtitle ? 2 + strlen(data["subtitle"]) : 0))+"s", "") + "\u2510";
            lines += ({ line });

            // Title Line 2
            line = "\u250c" + (fHeader ? "\u252c" : "\u2500") + "\u2500";
            line += "\u2524 " + (ansi?"%^WHITE%^BOLD%^":"") + data["title"];
            n += 5 + strlen(data["title"]);
            if (fSubtitle) {
                line += ":" + (ansi?"%^BOLD_OFF%^":"") + " " + data["subtitle"];
                n += 2 + strlen(data["subtitle"]);
            } else {
                line += (ansi?"%^BOLD_OFF%^":"");
            }
            line += " " + (ansi?"%^CYAN%^":"") + "\u251c\u2500";
            n += 3;
            line += sprintf("%'\u2500'"+sprintf("%d", width-2-n)+"s", "");
            line += (fHeader ? "\u252c" : "\u2500") + "\u2510";
            lines += ({ line });

            // Title Line 3
            line = "\u2502" + (fHeader ? "\u2502" : " ") + " ";
            line += "\u2514" + sprintf("%'\u2500'"+sprintf("%d", 2 + strlen(data["title"]) + (data["subtitle"] ? 2 + strlen(data["subtitle"]) : 0))+"s", "") + "\u2518 ";
            line += sprintf("%' '"+sprintf("%d", width-2-n)+"s", "");
            line += (fHeader ? "\u2502" : " ") + "\u2502";
            lines += ({ line });

        } else { // no title
            line = (ansi?"%^RESET%^CYAN%^":"") + "\u250c" + (fHeader ? "\u252c" : "\u2500") + sprintf("%'\u2500'"+sprintf("%d", width-4)+"s", "") + (fHeader ? "\u252c" : "\u2500") + "\u2510";
            lines += ({ line });
        }

        if (fHeader) {
            string format;
            // Header Lines
            if (data["header"]["align"] == "center" && data["header"]["columns"] == 1) {
                foreach (string item in data["header"]["items"]) {
                    line = "\u2502\u2502  " + (ansi?"%^RESET%^":"") + sprintf("%|"+sprintf("%d", width-8)+"s", item) + (ansi?"%^CYAN%^":"") + "  \u2502\u2502";
                    lines += ({ line });
                }
            } else {
                format = sizeof(data["header"]["items"]) > 0 ? format_page(data["header"]["items"], data["header"]["columns"], 4) : "";
                foreach (string l in explode(format, "\n")) {
                    line = "\u2502\u2502  " + (ansi ? "%^RESET%^" + l + "%^CYAN%^" : l) + "  \u2502\u2502";
                    lines += ({ line });
                }
            }
            // Header bottom line
            line = "\u2502\u2514" + sprintf("%'\u2500'"+sprintf("%d", width - 4)+"s", "") + "\u2518\u2502";
            lines += ({ line });
        }

        if (fBody) {
            string format;
            // Body top line
            line = "\u2502" + sprintf("%' '"+sprintf("%d", width - 2)+"s", "") + "\u2502";
            lines += ({ line });
            // Body Lines
            if (data["body"]["align"] == "center" && data["body"]["columns"] == 1) {
                foreach (string item in data["body"]["items"]) {
                    line = "\u2502   " + (ansi?"%^RESET%^":"") + sprintf("%|"+sprintf("%d", width - 8)+"s", item) + (ansi?"%^CYAN%^":"") + "   \u2502";
                    lines += ({ line });
                }
            } else {
                format = sizeof(data["body"]["items"]) > 0 ? format_page(data["body"]["items"], data["body"]["columns"], 4) : "";
                foreach (string l in explode(format, "\n")) {
                    line = "\u2502   " + (ansi ? "%^RESET%^" + l + "%^CYAN%^" : l) + "   \u2502";
                    lines += ({ line });
                }
            }
            // Body bottom line
            line = "\u2502";
            line += sprintf("%' '"+sprintf("%d", width - 2)+"s", "");
            line += "\u2502";
            lines += ({ line });
        }

        if (fFooter) {
            string *list = ({}), format;
            // Footer top line
            line = "\u2502\u250c";
            line += sprintf("%'\u2500'"+sprintf("%d", width - 4)+"s", "");
            line += "\u2510\u2502";
            lines += ({ line });
            // Footer Lines
            if (data["footer"]["align"] == "center" && data["footer"]["columns"] == 1) {
                foreach (string item in data["footer"]["items"]) {
                    line = "\u2502\u2502  " + (ansi?"%^RESET%^":"") + sprintf("%|"+sprintf("%d", width - 8)+"s", item) + (ansi?"%^CYAN%^":"") + "  \u2502\u2502";
                    lines += ({ line });
                }
            } else {
                format = sizeof(data["footer"]["items"]) > 0 ? format_page(data["footer"]["items"], data["footer"]["columns"], 4) : "";
                foreach (string l in explode(format, "\n")) {
                    line = "\u2502\u2502  " + (ansi ? "%^RESET%^" + l + "%^CYAN%^" : l) + "  \u2502\u2502";
                    lines += ({ line });
                }
            }
            // Bottom line
            line = "\u2514\u2534" + sprintf("%'\u2500'"+sprintf("%d", width - 4)+"s", "") + "\u2534\u2518" + (ansi?"%^RESET%^":"");
            lines += ({ line });
        } else {
            // Bottom line
            line = "\u2514" + sprintf("%'\u2500'"+sprintf("%d", width - 2)+"s", "") + "\u2518" + (ansi?"%^RESET%^":"");
            lines += ({ line });
        }
    } else { // screenreader
        if (fTitle) {
            line = data["title"] + (fSubtitle ? ": " + data["subtitle"] : "");
            lines += ({ line });
        }
        if (fHeader) {
            foreach (string item in data["header"]["items"]) {
                lines += ({ item });
            }
        }
        if (fBody) {
            foreach (string item in data["body"]["items"]) {
                lines += ({ item });
            }
        }
        if (fFooter) {
            foreach (string item in data["footer"]["items"]) {
                lines += ({ item });
            }
        }
    }

    return lines;
}