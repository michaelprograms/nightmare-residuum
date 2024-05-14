#include <config.h>

private mapping __ClassColors = ([
    "warrior": ({ ({ 220, 20, 60 }), ({ 192, 192, 192 }) }), // Crimson
    "mystic": ({ ({ 255, 215, 0 }), ({ 192, 192, 192 }) }), // Gold
    "scoundrel": ({ ({ 255, 99, 71 }), ({ 192, 192, 192 }) }), // Tomato
    "ranger": ({ ({ 34, 139, 34 }), ({ 192, 192, 192 }) }), // Forest Green
    "psionist": ({ ({ 65, 105, 225 }), ({ 192, 192, 192 }) }), // Royal Blue
    "paladin": ({ ({ 148, 0, 211 }), ({ 192, 192, 192 }) }), // Dark Violet
    "default": ({ ({ 191, 63, 191 }), ({ 63, 191, 191 }) }),
]);

mixed *query_character_border_colors () {
    object po = SEFUN->this_character();
    string c;

    if ((c = po->query_class()) && __ClassColors[c]) {
        return __ClassColors[c];
    } else {
        return __ClassColors["default"];
    }
}

nosave private mapping __BorderCharset = ([
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
        "tlr": "╭",
        "trr": "╮",
        "blr": "╰",
        "brr": "╯",
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
        "tlr": ".",
        "trr": ".",
        "blr": "'",
        "brr": "'",
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
        "tlr": " ",
        "trr": " ",
        "blr": " ",
        "brr": " ",
        // joints
        "t": " ",
        "b": " ",
        "l": " ",
        "r": " ",
    ]),
]);
mapping query_border_charset () {
    string type;
    if (SEFUN->query_account_setting("screenreader") == "on") {
        type = "screenreader";
    } else if (this_user()) {
        switch (SEFUN->this_user()->query_terminal("type")) {
        case "cmud": case "zmud":
            type = "US-ASCII";
            break;
        default:
            type = query_encoding();
            break;
        }
    } else {
        type = "utf-8";
    }
    return copy(__BorderCharset[type]);
}

// create a unicode border around action output
// https://en.wikipedia.org/wiki/Box_Drawing
//
// mapping data = ([
//     "title": "TITLE" || UNDEFINED,
//     "subtitle": "Subtitle" || UNDEFINED,
//     "ansi": 1 || UNDEFINED,
//     "header": ({
//         ([
//             "header": ({ /* array of strings */ }),
//             "items": ({ /* array of strings */ }),
//             "columns": 2 || ({ /* array of integer ratios */ }),
//             "align": "left || center",
//         ]),
//     }) || UNDEFINED,
//     "body": ({
//         ([
//             "header": ({ /* array of strings */ }),
//             "items": ({ /* array of strings */ }),
//             "columns": 2 || ({ /* array of integer ratios */ }),
//             "align": "left || center",
//         ]),
//     }) || UNDEFINED,
//     "footer": ({
//         ([
//             "header": ({ /* array of strings */ }),
//             "items": ({ /* array of strings */ }),
//             "columns": 2 || ({ /* array of integer ratios */ }),
//             "align": "left || center",
//         ]),
//     }) || UNDEFINED,
// ]);
private string *format_border_item (mapping item, int width, string ansi, string left, string right) {
    string *lines = ({ }), line = "", format;
    int columnWidth;

    if (undefinedp(item["columns"])) {
        item["columns"] = 2;
    }

    // header
    if (arrayp(item["header"])) {
        columnWidth = arrayp(item["columns"]) ? sizeof(item["columns"]) : item["columns"];
        if (sizeof(item["header"]) > columnWidth) {
            item["header"] = item["header"][0..columnWidth-1];
        }
        if (ansi) {
            item["header"] = map(item["header"], (: "%^RESET%^BOLD%^UNDERLINE%^"+$1+"%^RESET%^" :));
        }
        line = left + "  " + SEFUN->format_page(item["header"], item["columns"], 4, (item["align"] == "center"), ansi) + "  " + right;
        lines += ({ line });
    }
    // items
    format = sizeof(item["items"]) > 0 ? SEFUN->format_page(item["items"], item["columns"], 4, (item["align"] == "center"), ansi) : "";
    foreach (string l in explode(format, "\n") - ({""})) {
        lines += ({ left + "  " + l + "  " + right });
    }

    return lines;
}
string *format_border (mapping data, mapping b, int width, string ansi) {
    string *lines = ({ }), line;
    string *colors, *colors2;
    mixed *borderColors;
    int headerStart, headerEnd, footerLine;
    int i, l;
    int fTitle = !!(!undefinedp(data["title"]) && data["title"]);
    int fSubtitle = !undefinedp(data["subtitle"]) && sizeof(data["subtitle"]);
    int lSubtitle = sizeof(SEFUN->strip_colour(data["subtitle"]));
    int fHeader = !!(!undefinedp(data["header"]) && data["header"]);
    int fBody = !!(!undefinedp(data["body"]) && data["body"]);
    int fFooter = !!(!undefinedp(data["footer"]) && data["footer"]);

    string radius = "r";

    if (fHeader && mapp(data["header"])) {
        data["header"] = ({ data["header"] });
    }
    if (fBody && mapp(data["body"])) {
        data["body"] = ({ data["body"] });
    }
    if (fFooter && mapp(data["footer"])) {
        data["footer"] = ({ data["footer"] });
    }

    if (ansi && arrayp(data["borderColors"])) {
        borderColors = data["borderColors"];
        colors = SEFUN->color_gradient(borderColors[0..1]..., width);
        colors2 = ({ });
        for (i = sizeof(colors)-1; i >= 0; i --) {
            colors2 += ({ colors[i] });
        }
    } else if (ansi == "256") {
        borderColors = query_character_border_colors();
        colors = SEFUN->color_gradient(borderColors[0..1]..., width);
        colors2 = ({ });
        for (i = sizeof(colors)-1; i >= 0; i --) {
            colors2 += ({ colors[i] });
        }
    } else {
        colors = allocate(width, "");
        colors2 = colors;
    }

    if (fTitle) {
        int n = 0;
        // Title Line 1
        line = "   " + b["tl"+radius] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (lSubtitle ? 2 + lSubtitle : 0), "") + b["tr"+radius];
        if (ansi == "256") {
            line = line[0..2] + SEFUN->apply_gradient(line[3..], colors);
        } else if (ansi) {
            line = line[0..2] + "\e[36m" + line[3..] + "\e[0;37;40m";
        }
        lines += ({ line });
        // Title Line 2
        line = b["tl"+radius] + (fHeader ? b["t"] : b["h"]) + b["h"];
        line += b["r"] + " " + data["title"];
        n += 5 + strlen(data["title"]);
        if (fSubtitle) {
            line += ": " + data["subtitle"];
            n += 2 + fSubtitle;
        }
        l = n;
        line += " " + b["l"] + b["h"];
        n += 3;
        line += sprintf("%'"+b["h"]+"'*s", width-2-n + (fSubtitle - lSubtitle), "");
        line += (fHeader ? b["t"] : b["h"]) + b["tr"+radius];
        if (ansi == "256") {
            line = SEFUN->apply_gradient(line[0..3], colors[0..3]) + "\e[0;37;40;1m" + replace_string(line[4..l], ":", ":\e[22m") + SEFUN->apply_gradient(line[l+1..], colors[l+1-(fSubtitle - lSubtitle)..]);
        } else if (ansi) {
            line = "\e[36m" + line[0..4] + "\e[0;37;40;1m" + line[5..l-1] + "\e[22;36m" + line[l..] + "\e[0;37;40m";
        }
        lines += ({ line });

        // Title Line 3
        line = b["v"] + (fHeader ? b["v"] : " ") + " ";
        line += b["bl"+radius] + sprintf("%'"+b["h"]+"'*s", 2 + strlen(data["title"]) + (lSubtitle ? 2 + lSubtitle : 0), "") + b["br"+radius];
        line += sprintf("%*s", width - 1 - n + (fSubtitle - lSubtitle), "");
        line += (fHeader ? b["v"] : " ") + b["v"];
        if (ansi == "256") {
            line = SEFUN->apply_gradient(line[0..fHeader], colors[0..fHeader]) +
                line[1+fHeader..2] +
                SEFUN->apply_gradient(line[3..l+1], colors[3..l+1]) +
                line[l+2..<2+fHeader] +
                SEFUN->apply_gradient(line[<1+fHeader..<1], colors[<1+fHeader..<1]);
        } else if (ansi) {
            line = "\e[36m" + line[0..fHeader] + "\e[0;37;40m" +
            line[1+fHeader..2] +
            "\e[36m" + line[3..l+1] + "\e[0;37;40m" +
            line[l+2..<2+fHeader] +
            "\e[36m" + line[<1+fHeader..<1] + "\e[0;37;40m";
        }
        lines += ({ line });

    } else { // no title
        line = b["tl"+radius] + (fHeader ? b["t"] : b["h"]) + sprintf("%'"+b["h"]+"'*s", width-4, "") + (fHeader ? b["t"] : b["h"]) + b["tr"+radius];
        if (ansi == "256") {
            line = SEFUN->apply_gradient(line, colors);
        } else if (ansi) {
            line = "\e[36m" + line + "\e[0;37;40m";
        }
        lines += ({ line });
    }
    headerStart = sizeof(lines);

    if (fHeader) {
        for (i = 0, l = sizeof(data["header"]); i < l; i ++) {
            lines += format_border_item(data["header"][i], width, ansi, b["v"]+b["v"], b["v"]+b["v"]);
            if (i < l-1) {
                lines += ({ b["v"] + b["v"] + sprintf("%*s", width-4, "") + b["v"] + b["v"] });
            }
        }
        // Header bottom line
        line = b["v"] + b["bl"+radius] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["br"+radius] + b["v"];
        lines += ({ line });
        headerEnd = sizeof(lines) - 1;
    }

    if (fBody) {
        // Body top line
        line = b["v"] + sprintf("%*s", width-2, "") + b["v"];
        lines += ({ line });
        foreach (mapping child in data["body"]) {
            lines += format_border_item(child, width, ansi, b["v"]+" ", " "+b["v"]);
            lines += ({ b["v"] + sprintf("%*s", width-2, "") + b["v"] });
        }
    }

    if (fFooter) {
        // Footer top line
        line = b["v"] + b["tl"+radius] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["tr"+radius] + b["v"];
        lines += ({ line });
        footerLine = sizeof(lines) - 1;
        // Footer Items
        for (i = 0, l = sizeof(data["footer"]); i < l; i ++) {
            if (i > 0) {
                lines += ({ b["v"] + b["v"] + sprintf("%*s", width-4, "") + b["v"] + b["v"] });
            }
            lines += format_border_item(data["footer"][i], width, ansi, b["v"]+b["v"], b["v"]+b["v"]);
        }
        // Footer Bottom line
        line = b["bl"+radius] + b["b"] + sprintf("%'"+b["h"]+"'*s", width-4, "") + b["b"] + b["br"+radius];
    } else {
        // Footer Bottom line
        line = b["bl"+radius] + sprintf("%'"+b["h"]+"'*s", width-2, "") + b["br"+radius];
    }
    if (ansi == "256") {
        line = SEFUN->apply_gradient(line, colors2);
    } else if (ansi) {
        line = "\e[36m" + line + "\e[0;37;40m";
    }
    lines += ({ line });
    l = sizeof(lines);

    // Colorize edges and separators
    if (ansi == "256") {
        string *left = SEFUN->color_gradient(borderColors[0], borderColors[1], l - (headerStart-1))[1..<2];
        string *right = ({ });
        for (i = sizeof(left)-1; i >= 0; i --) {
            right += ({ left[i] });
        }
        for (i = headerStart; i < l-1; i ++) {
            if (i < headerEnd || (i > footerLine && footerLine > 0)) {
                lines[i] = "\e[38;2;"+left[i-headerStart]+"m"+lines[i][0..1]+"\e[0;37;40m" + lines[i][2..<3] + "\e[38;2;"+right[i-headerStart]+"m" + lines[i][<2..] + "\e[0;37;40m";
            } else if (i == headerEnd || i == footerLine) {
                mixed *gradient;
                mixed *lside, *rside;
                lside = map(explode(left[i-headerStart], ";"), (: to_int($1) :));
                rside = map(explode(right[i-headerStart], ";"), (: to_int($1) :));
                gradient = SEFUN->color_gradient(lside, rside, width);
                lines[i] = SEFUN->apply_gradient(lines[i], gradient);
            } else if (i < footerLine || !footerLine) {
                lines[i] = "\e[38;2;"+left[i-headerStart]+"m"+lines[i][0..0]+"\e[0;37;40m" + lines[i][1..<2] + "\e[38;2;"+right[i-headerStart]+"m" + lines[i][<1..] + "\e[0;37;40m";
            }
        }
    } else if (ansi) {
        for (i = headerStart; i < l-1; i ++) {
            if (i == headerEnd || i == footerLine) {
                lines[i] = "\e[36m" + lines[i] + "\e[0;37;40m";
            } else if (i < headerEnd || i > footerLine) {
                lines[i] = "\e[36m"+lines[i][0..1]+"\e[0;37;40m" + lines[i][2..<3] + "\e[36m" + lines[i][<2..] + "\e[0;37;40m";
            } else if (i < footerLine) {
                lines[i] = "\e[36m"+lines[i][0..0]+"\e[0;37;40m" + lines[i][1..<2] + "\e[36m" + lines[i][<1..] + "\e[0;37;40m";
            }
        }
    }

    return lines;
}

void border (mapping data) {
    mapping b = query_border_charset();
    int width = to_int(SEFUN->query_account_setting("width")) || DEFAULT_WIDTH;
    string ansi, *result;
    if (undefinedp(data["ansi"]) && SEFUN->query_account_setting("ansi") == "on") {
        ansi = this_user()->query_terminal_color();
    }

    result = format_border(data, b, width, ansi);
    SEFUN->this_user()->handle_pager(result);
}

private varargs string *format_tree (string key, mixed value, mapping b, int indent, int index, int maxIndex, mapping prefix) {
    string *result = ({ }), tmp = "";
    string *ids;
    int i, l;

    if (indent > 0) {
        for (i = 0; i < indent; i ++) {
            if (i > 0) {
                if (prefix[i]) {
                    tmp += "  ";
                } else {
                    tmp += b["v"] + " ";
                }
            }
        }
        if (index == maxIndex) {
            tmp += b["bl"];
            prefix[indent] = 1;
        } else {
            tmp += b["l"];
        }
        tmp += b["h"];
    }

    tmp += key;
    indent ++;
    if (stringp(value)) {
        tmp += value;
    }
    result += ({ tmp });

    if (mapp(value)) {
        ids = sort_array(keys(value), 1);
        l = sizeof(ids);
        for (i = 0; i < l; i ++) {
            result += format_tree(i + ". " + ids[i], value[ids[i]], b, indent, i, l-1, prefix);
            prefix[indent] = 0;
        }
    }

    return result;
}
string *tree (mapping value) {
    string *result = ({ });
    mapping b = query_border_charset();
    string *ids;
    int i, l;

    if (!mapp(value) || !sizeof(value)) {
        error("Bad argument 1 to border->tree");
    }

    ids = sort_array(keys(value), (: to_int(explode($1," ")[0]) < to_int(explode($2," ")[0]) ? 0 : 1 :));
    l = sizeof(ids);
    for (i = 0; i < l; i ++) {
        result += format_tree(i + ". " + ids[i], value[ids[i]], b, 0, i, l-1, ([ ]));
    }

    return result;
}