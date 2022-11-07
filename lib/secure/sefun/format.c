private object query_previous_object () {
    object po = previous_object(-1)[<1];
    if (strsrch(D_TEST, base_name(po)) == 0) po = previous_object(-1)[<2];
    return po;
}

private string query_account_setting (string setting) {
    object account;

    if (!(account = query_previous_object()->query_account())) return 0;

    return account->query_setting(setting);
}

private mixed *query_character_border_colors () {
    object po = query_previous_object()->query_character();
    int *arr1, *arr2 = ({ 192, 192, 192 });

    switch (po->query_class()) {
        case "warrior":
            arr1 = ({ 220, 20, 60 });   // Crimson
            break;
        case "templar":
            arr1 = ({ 255, 215, 0 });   // Gold
            break;
        case "scoundrel":
            arr1 = ({ 255, 99, 71 });  // Tomato
            break;
        case "ranger":
            arr1 = ({ 34, 139, 34 });   // Forest Green
            break;
        case "psionist":
            arr1 = ({ 65, 105, 225 });  // Royal Blue
            break;
        case "paladin":
            arr1 = ({ 148, 0, 211 });   // Dark Violet
            break;
        default:
            arr1 = ({ 191, 63, 191 });
            arr2 = ({ 63, 191, 191 });
            break;
    }

    return ({ arr1, arr2 });
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
                row += sprintf("%' '*s", w*(columns-j), " ");
                break;
            }
            // check text length without ANSI color
            if (sizeof(tmp = SEFUN->strip_colour(items[i + j])) > w) {
                // use stripped text when its longer than w
                if (columns == 1) {
                    string *wrapped = explode(SEFUN->wrap(""+items[i + j], w, 0), "\n");
                    foreach (string line in wrapped) {
                        if (center) {
                            row += sprintf("%|*s", w, ""+line) + "\n";
                        } else {
                            row += sprintf("%-*s", w, ""+line) + "\n";
                        }
                    }
                } else {
                    row += tmp[0..w-1];
                }
            } else {
                // account for any color codes in the text
                int diff = sizeof(items[i+j]) - sizeof(tmp);

                if (center) {
                    row += sprintf("%|*s", w+diff, ""+items[i + j]);
                } else {
                    row += sprintf("%-*s", w+diff, ""+items[i + j]);
                }

                // when color codes present and we're not displaying a RESET, add one
                if (diff > 0 && items[i + j][<9..] != "%^RESET%^") row += "%^RESET%^";
            }
        }
        if (r) row += sprintf("%' '*s", r, "");
        rows += ({ row });
    }
    return implode(rows, "\n");
}

string format_syntax (string text) {
    string s = text;

    if (!stringp(text)) error("Bad argument 1 to format->format_syntax");

    if (!regexp(s, "^<")) s = "<" + s;
    if (!regexp(s, ">$")) s = s + ">";
    if (query_account_setting("ansi") == "on") {
        s = replace_string(s, "<", "%^CYAN%^<");
        s = replace_string(s, "[", "%^BOLD%^[%^BOLD_OFF%^");
        s = replace_string(s, "]", "%^BOLD%^]%^BOLD_OFF%^");
        s = replace_string(s, "(", "%^RESET%^(%^CYAN%^");
        s = replace_string(s, "|", "%^RESET%^|%^CYAN%^");
        s = replace_string(s, ")", "%^RESET%^)%^CYAN%^");
        s = replace_string(s, ">", ">%^RESET%^");
    }
    return s;
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

    for (int i = s; i >= 0; i --) {
        result = digits[i] + (i != s && (s-i)%3 == 0 ? "," : "") + result;
    }

    return (neg ? "-" : "") + result;
}

// create a unicode border around action output
// https://en.wikipedia.org/wiki/Box_Drawing
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
mapping query_border_charset () {
    string type;
    if (query_account_setting("screenreader") == "on") {
        type = "screenreader";
    } else {
        type = query_encoding();
    }
    return ([
        "utf-8": ([
            // lines
            "h": "─",
            "v": "│",
            // diagonals
            "dd": "╲",
            "du": "╱",
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
            "x": "╳",
            "+": "┼",

        ]),
        "US-ASCII": ([
            // lines
            "h": "-",
            "v": "|",
            // diagonals
            "dd": "\\",
            "du": "/",
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
            "x": "x",
            "+": "+",
        ]),
        "screenreader": ([
            // lines
            "h": " ",
            "v": " ",
            // diagonals
            "dd": " ",
            "du": " ",
            // corners
            "tl": " ",
            "tr": " ",
            "br": " ",
            "bl": " ",
            // joints
            "t": " ",
            "b": " ",
            "l": " ",
            "r": " ",
        ]),
    ])[type];
}
string *format_border (mapping data) {
    string *lines = ({ }), line, *linesBody = ({ });

    int fTitle = !undefinedp(data["title"]) && data["title"];
    int fSubtitle = !undefinedp(data["subtitle"]) && data["subtitle"];
    int fHeader = !undefinedp(data["header"]) && data["header"];
    int fBody = !undefinedp(data["body"]) && data["body"];
    int fFooter = !undefinedp(data["footer"]) && data["footer"];

    mapping b; // border charset
    int ansi, width, n;
    string *colors, *colors2, *colorsBody, *colorsBody2;
    mixed *borderColors;
    string left, right;
    string format;

    if (fSubtitle) {
        data["subtitle"] = SEFUN->strip_colour(data["subtitle"]);
    }

    b = query_border_charset();
    ansi = query_account_setting("ansi") == "on";
    width = to_int(query_account_setting("width"));
    n = 0;

    if (ansi && query_account_setting("screenreader") != "on") {
        borderColors = query_character_border_colors();
        colors = SEFUN->color_gradient(borderColors[0], borderColors[1], width);
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
        line = "   " + b["tl"] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (fSubtitle ? 2 + strlen(data["subtitle"]) : 0), "") + b["tr"];
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
        line += sprintf("%'"+b["h"]+"'*s", width-2-n, "");
        line += (fHeader ? b["t"] : b["h"]) + b["tr"];
        if (ansi) {
            line = SEFUN->apply_gradient(line[0..4], colors[0..4]) + "\e[0;37;40;1m" + replace_string(line[5..titleLength-1], ":", ":\e[22m") + SEFUN->apply_gradient(line[titleLength..], colors[titleLength..]);
        }
        lines += ({ line });

        // Title Line 3
        line = b["v"] + (fHeader ? b["v"] : " ") + " ";
        line += b["bl"] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (data["subtitle"] ? 2 + strlen(data["subtitle"]) : 0), "") + b["br"];
        line += sprintf("%' '*s", width-1-n, "");
        line += (fHeader ? b["v"] : " ") + b["v"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        lines += ({ line });

    } else { // no title
        line = b["tl"] + (fHeader ? b["t"] : b["h"]) + sprintf("%'"+b["h"]+"'*s", width-4, "") + (fHeader ? b["t"] : b["h"]) + b["tr"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        lines += ({ line });
    }

    if (fHeader) {
        left = b["v"] + b["v"];
        right = b["v"] + b["v"];
        if (ansi) {
            left = SEFUN->apply_gradient(left, colors[0..1]);
            right = SEFUN->apply_gradient(right, colors[<2..<1]);
        }
        // Header header
        if (stringp(data["header"]["header"])) {
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["header"]["header"] + (ansi?"\e[22m":"") + sprintf("%' '*s", width-8-strlen(data["header"]["header"]), "") + "  " + right;
            lines += ({ line });
        } else if (arrayp(data["header"]["header"])) {
            if (!data["header"]["columns"]) {
                data["header"]["columns"] = 2;
            }
            if (sizeof(data["header"]["header"]) > data["header"]["columns"]) {
                data["header"]["header"] = data["header"]["header"][0..data["header"]["columns"]-1];
            }
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + format_page(data["header"]["header"], data["header"]["columns"], 4, (data["header"]["align"] == "center")) + (ansi?"\e[22m":"") + "  " + right;
            lines += ({ line });
        }
        // Header lines
        format = sizeof(data["header"]["items"]) > 0 ? format_page(data["header"]["items"], data["header"]["columns"], 4, (data["header"]["align"] == "center")) : "";
        foreach (string l in explode(format, "\n")) {
            line = left + "  " + l + "  " + right;
            lines += ({ line });
        }
        // Header bottom line
        line = b["v"] + b["bl"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["br"] + b["v"];
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
        line = sprintf("%' '*s", width-2, "");
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors);
        }
        linesBody += ({ line });
        foreach (mapping child in data["body"]) {
            // Body child header
            if (stringp(child["header"])) {
                line = "   " + (ansi?"\e[0;37;40;1m":"") + child["header"] + (ansi?"\e[22m":"") + sprintf("%' '*s", width-8-strlen(child["header"]), "") + "   ";
                linesBody += ({ line });
            } else if (arrayp(child["header"])) {
                if (!child["columns"]) {
                    child["columns"] = 2;
                }
                if (sizeof(child["header"]) > child["columns"]) {
                    child["header"] = child["header"][0..child["columns"]-1];
                }
                line = "   " + (ansi?"\e[0;37;40;1m":"") + format_page(child["header"], child["columns"], 4, (child["align"] == "center")) + (ansi?"\e[22m":"") + "   ";
                linesBody += ({ line });
            }
            // Body child lines
            format = sizeof(child["items"]) > 0 ? format_page(child["items"], child["columns"], 4, (child["align"] == "center")) : "";
            foreach (string l in explode(format, "\n")) {
                line = "   " + l + "   ";
                linesBody += ({ line });
            }
            // Body child bottom line
            line = sprintf("%' '*s", width-2, "");
            linesBody += ({ line });
        }
        if (ansi && query_account_setting("screenreader") != "on") {
            colorsBody = SEFUN->color_gradient(borderColors[0], borderColors[1], sizeof(linesBody) + 2);
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
        left = b["v"] + b["v"];
        right = b["v"] + b["v"];
        if (ansi) {
            left = SEFUN->apply_gradient(left, colors2[0..1]);
            right = SEFUN->apply_gradient(right, colors2[<2..<1]);
        }

        // Footer top line
        line = b["v"] + b["tl"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["tr"] + b["v"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors2);
        }
        lines += ({ line });
        // Footer header
        if (stringp(data["footer"]["header"])) {
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + data["footer"]["header"] + (ansi?"\e[22m":"") + sprintf("%' '*s", width-8-strlen(data["footer"]["header"]), "") + "  " + right;
            lines += ({ line });
        } else if (arrayp(data["footer"]["header"])) {
            if (!data["footer"]["columns"]) {
                data["footer"]["columns"] = 2;
            }
            if (sizeof(data["footer"]["header"]) > data["footer"]["columns"]) {
                data["footer"]["header"] = data["footer"]["header"][0..data["footer"]["columns"]-1];
            }
            line = left + "  " + (ansi?"\e[0;37;40;1m":"") + format_page(data["footer"]["header"], data["footer"]["columns"], 4, (data["footer"]["align"] == "center")) + (ansi?"\e[22m":"") + "  " + right;
            lines += ({ line });
        }
        // Footer lines
        format = sizeof(data["footer"]["items"]) > 0 ? format_page(data["footer"]["items"], data["footer"]["columns"], 4, (data["footer"]["align"] == "center")) : "";
        foreach (string l in explode(format, "\n")) {
            line = left + "  " + l + "  " + right;
            lines += ({ line });
        }
        // Border Bottom line
        line = b["bl"] + b["b"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["b"] + b["br"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors2);
        }
        lines += ({ line });
    } else {
        // Border Bottom line
        line = b["bl"] + sprintf("%'"+b["h"]+"'*s", width-2, "") + b["br"];
        if (ansi) {
            line = SEFUN->apply_gradient(line, colors2);
        }
        lines += ({ line });
    }

    return lines;
}