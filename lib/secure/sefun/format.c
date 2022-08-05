private string query_account_setting (string setting) {
    object po = previous_object(-1)[<1], account;

    if (strsrch(D_TEST, base_name(po)) == 0) po = previous_object(-1)[<2];

    if (!(account = po->query_account())) return 0;

    return account->query_setting(setting);
}

varargs string format_page (string *items, int columns, int pad, int center) {
    int width, w, i, j, n, r;
    string *rows = ({});

    if (!arrayp(items) || !sizeof(items)) error("Bad argument 1 to format->format_page");

    if (!columns) columns = 2;
    width = to_int(query_account_setting("width")) - pad * 2;
    w = width / columns; // width of column
    n = sizeof(items); // number of columns
    r = width - (w * columns); // remainder
    items = map(items, (: "" + $1 :));

    for (i = 0; i < n; i += columns) {
        string row = "";
        for (j = 0; j < columns; j ++) {
            string tmp;
            if (i + j >= n) { // ran out of columns to fill final row
                row += sprintf("%' '"+sprintf("%d", w*(columns-j))+"s", " ");
                break;
            }
            // check text length without ANSI color
            if (sizeof(tmp = SEFUN->strip_colour(items[i + j])) > w) {
                // use stripped text when its longer than w
                row += tmp[0..w-1];
            } else {
                // account for any color codes in the text
                int diff = sizeof(items[i+j]) - sizeof(tmp);

                if (center) {
                    row += sprintf("%|"+sprintf("%d", w+diff)+"s", ""+items[i + j]);
                } else {
                    row += sprintf("%-"+sprintf("%d", w+diff)+"s", ""+items[i + j]);
                }

                // when color codes present and we're not displaying a RESET, add one
                if (diff > 0 && items[i + j][<9..] != "%^RESET%^") row += "%^RESET%^";
            }
        }
        if (r) row += sprintf("%' '"+sprintf("%d", r)+"s", "");
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
// https://en.wikipedia.org/wiki/Box-drawing_character#Box_Drawing
//
// mapping data = ([
// /*
//     "title": "TITLE",
//     "subtitle": "Subtitle",
//     "header": ([
//         "header": "string" || ({ /* array of strings */ })
//         "items": ({ }),
//         "columns": 2,
//         "align": "left|center",
//     ]),
//     "body": ([
//         "header": "string" || ({ /* array of strings */ }),
//         "items": ({ /* array of strings */ }),
//         "columns": 2,
//         "align": "left|center",
//     ]) || ({ /* array of mappings */ }),
//     "footer": ([
//         "header": "string" || ({ /* array of strings */ }),
//         "items": ({ /* array of strings */ }),
//         "columns": 2,
//         "align": "left|center",
//     ]),
// */
// ]);
private nosave mapping __Border = ([
    "utf-8": ([
        // lines
        "h": "─",
        "v": "│",
        // corners
        "tl": "┌",
        "tr": "┐",
        "br": "┘",
        "bl": "└",
        // joints
        "t": "┬",
        "b": "┴",
        "l": "├",
        "r": "┤",
    ]),
    "US-ASCII": ([
        // lines
        "h": "-",
        "v": "|",
        // corners
        "tl": ".",
        "tr": ".",
        "br": "'",
        "bl": "'",
        // joints
        "t": "-",
        "b": "-",
        "l": "|",
        "r": "|",
    ]),
]);
string *format_border (mapping data) {
    string *lines = ({ }), line, *linesBody = ({ });

    int fTitle = !undefinedp(data["title"]);
    int fSubtitle = !undefinedp(data["subtitle"]);
    int fHeader = !undefinedp(data["header"]);
    int fBody = !undefinedp(data["body"]);
    int fFooter = !undefinedp(data["footer"]);

    if (query_account_setting("screenreader") != "on") {
        mapping b = __Border[query_encoding()];
        int width, ansi, n;
        string *colors, *colors2, *colorsBody, *colorsBody2;
        int *c1, *c2;

        ansi = query_account_setting("ansi") == "on";
        width = to_int(query_account_setting("width"));
        n = 0;

        if (ansi) {
            c1 = ({ 191, 63, 191 }); // pink
            c2 = ({ 63, 191, 191 }); // cyan
            colors = SEFUN->color_gradient(c1, c2, width);
            colors2 = ({ });
            for (int i = sizeof(colors)-1; i >= 0; i --) {
                colors2 += ({ colors[i] });
            }
        } else {
            colors = allocate(width, "");
            colors2 = colors;
        }

        if (fTitle) {
            int titleLength = 0;

            // Title Line 1
            line = "   " + b["tl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", 2 + strlen(data["title"]) + (fSubtitle ? 2 + strlen(data["subtitle"]) : 0))+"s", "") + b["tr"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors);
            }
            lines += ({ line });

            // Title Line 2
            line = b["tl"] + (fHeader ? b["t"] : b["h"]) + b["h"];
            line += b["r"] + " " + data["title"];
            n += 5 + strlen(data["title"]);
            if (fSubtitle) {
                line += ": " + data["subtitle"];
                n += 2 + strlen(data["subtitle"]);
            }
            titleLength = n;
            line += " " + b["l"] + b["h"];
            n += 3;
            line += sprintf("%'"+b["h"]+"'"+sprintf("%d", width-2-n)+"s", "");
            line += (fHeader ? b["t"] : b["h"]) + b["tr"];
            if (ansi) {
                line = SEFUN->apply_gradient(line[0..4], colors[0..4]) + "\e[0;37;40;1m" + replace_string(line[5..titleLength-1], ":", ":\e[22m") + SEFUN->apply_gradient(line[titleLength..], colors[titleLength..]);
            }
            lines += ({ line });

            // Title Line 3
            line = b["v"] + (fHeader ? b["v"] : " ") + " ";
            line += b["bl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", 2 + strlen(data["title"]) + (data["subtitle"] ? 2 + strlen(data["subtitle"]) : 0))+"s", "") + b["br"];
            line += sprintf("%' '"+sprintf("%d", width-1-n)+"s", "");
            line += (fHeader ? b["v"] : " ") + b["v"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors);
            }
            lines += ({ line });

        } else { // no title
            line = b["tl"] + (fHeader ? b["t"] : b["h"]) + sprintf("%'"+b["h"]+"'"+sprintf("%d", width-4)+"s", "") + (fHeader ? b["t"] : b["h"]) + b["tr"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors);
            }
            lines += ({ line });
        }

        if (fHeader) {
            string format;
            int center = (data["header"]["align"] == "center");
            string left, right;

            left = b["v"] + b["v"];
            right = b["v"] + b["v"];
            if (ansi) {
                left = SEFUN->apply_gradient(left, colors[0..1]);
                right = SEFUN->apply_gradient(right, colors[<2..<1]);
            }
            // Header header
            if (stringp(data["header"]["header"])) {
                line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["header"]["header"] + (ansi?"\e[22m":"") + sprintf("%' '"+sprintf("%d", width - 8 - strlen(data["header"]["header"]))+"s", "") + "  " + right;
                lines += ({ line });
            } else if (arrayp(data["header"]["header"])) {
                if (!data["header"]["columns"]) {
                    data["header"]["columns"] = 2;
                }
                if (sizeof(data["header"]["header"]) > data["header"]["columns"]) {
                    data["header"]["header"] = data["header"]["header"][0..data["header"]["columns"]-1];
                }
                line = left + "  " + (ansi?"\e[0;37;40;1m":"") + format_page(data["header"]["header"], data["header"]["columns"], 4, center) + (ansi?"\e[22m":"") + "  " + right;
                lines += ({ line });
            }
            // Header lines
            format = sizeof(data["header"]["items"]) > 0 ? format_page(data["header"]["items"], data["header"]["columns"], 4, center) : "";
            foreach (string l in explode(format, "\n")) {
                line = left + "  " + l + "  " + right;
                lines += ({ line });
            }
            // Header bottom line
            line = b["v"] + b["bl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 4)+"s", "") + b["br"] + b["v"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors);
            }
            lines += ({ line });
        }

        if (fBody && mapp(data["body"])) {
            data["body"] = ({ data["body"] });
        }
        if (fBody && arrayp(data["body"])) {
            // Body top line
            line = sprintf("%' '"+sprintf("%d", width - 2)+"s", "");
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors);
            }
            linesBody += ({ line });
            foreach (mapping child in data["body"]) {
                string format;
                int center = (child["align"] == "center");

                // Body child header
                if (stringp(child["header"])) {
                    line = "   " + (ansi?"\e[0;37;40;1m":"") + child["header"] + (ansi?"\e[22m":"") + sprintf("%' '"+sprintf("%d", width - 8 - strlen(child["header"]))+"s", "") + "   ";
                    linesBody += ({ line });
                } else if (arrayp(child["header"])) {
                    if (!child["columns"]) {
                        child["columns"] = 2;
                    }
                    if (sizeof(child["header"]) > child["columns"]) {
                        child["header"] = child["header"][0..child["columns"]-1];
                    }
                    line = "   " + (ansi?"\e[0;37;40;1m":"") + format_page(child["header"], child["columns"], 4, center) + (ansi?"\e[22m":"") + "   ";
                    linesBody += ({ line });
                }
                // Body child lines
                format = sizeof(child["items"]) > 0 ? format_page(child["items"], child["columns"], 4, center) : "";
                foreach (string l in explode(format, "\n")) {
                    line = "   " + l + "   ";
                    linesBody += ({ line });
                }
                // Body child bottom line
                line = sprintf("%' '"+sprintf("%d", width - 2)+"s", "");
                linesBody += ({ line });
            }
            if (ansi) {
                string left, right;
                colorsBody = SEFUN->color_gradient(c1, c2, sizeof(linesBody) + 2);
                colorsBody = colorsBody[1..<2];
                colorsBody2 = ({ });
                for (int i = sizeof(colorsBody)-1; i >= 0; i --) {
                    colorsBody2 += ({ colorsBody[i] });
                }
                for (int i = sizeof(linesBody)-1; i >= 0; i --) {
                    left = SEFUN->apply_gradient(b["v"], ({ colorsBody[i] }));
                    right = SEFUN->apply_gradient(b["v"], ({ colorsBody2[i] }));
                    linesBody[i] = left + linesBody[i] + right;
                }
            } else {
                for (int i = sizeof(linesBody)-1; i >= 0; i --) {
                    linesBody[i] = b["v"] + linesBody[i] + b["v"];
                }
            }

            lines += linesBody;
        }

        if (fFooter) {
            string format;
            int center = (data["footer"]["align"] == "center");
            string left, right;

            left = b["v"] + b["v"];
            right = b["v"] + b["v"];
            if (ansi) {
                left = SEFUN->apply_gradient(left, colors2[0..1]);
                right = SEFUN->apply_gradient(right, colors2[<2..<1]);
            }

            // Footer top line
            line = b["v"] + b["tl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 4)+"s", "") + b["tr"] + b["v"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors2);
            }
            lines += ({ line });
            // Footer header
            if (stringp(data["footer"]["header"])) {
                line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["footer"]["header"] + (ansi?"\e[22m":"") + sprintf("%' '"+sprintf("%d", width - 8 - strlen(data["footer"]["header"]))+"s", "") + "  " + right;
                lines += ({ line });
            } else if (arrayp(data["footer"]["header"])) {
                if (!data["footer"]["columns"]) {
                    data["footer"]["columns"] = 2;
                }
                if (sizeof(data["footer"]["header"]) > data["footer"]["columns"]) {
                    data["footer"]["header"] = data["footer"]["header"][0..data["footer"]["columns"]-1];
                }
                line = left + "  " + (ansi?"\e[0;37;40;1m":"") + format_page(data["footer"]["header"], data["footer"]["columns"], 4, center) + (ansi?"\e[22m":"") + "  " + right;
                lines += ({ line });
            }
            // Footer lines
            format = sizeof(data["footer"]["items"]) > 0 ? format_page(data["footer"]["items"], data["footer"]["columns"], 4, center) : "";
            foreach (string l in explode(format, "\n")) {
                line = left + "  " + l + "  " + right;
                lines += ({ line });
            }
            // Border Bottom line
            line = b["bl"] + b["b"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 4)+"s", "") + b["b"] + b["br"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors2);
            }
            lines += ({ line });
        } else {
            // Border Bottom line
            line = b["bl"] + sprintf("%'"+b["h"]+"'"+sprintf("%d", width - 2)+"s", "") + b["br"];
            if (ansi) {
                line = SEFUN->apply_gradient(line, colors2);
            }
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
        if (fBody && mapp(data["body"])) {
            data["body"] = ({ data["body"] });
        }
        if (fBody && arrayp(data["body"])) {
            foreach (mapping child in data["body"]) {
                lines += ({ child["header"] });
                foreach (string item in child["items"]) {
                    lines += ({ item });
                }
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