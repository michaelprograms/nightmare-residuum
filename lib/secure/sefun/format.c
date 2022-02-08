#include "sefun.h"

varargs string format_header_bar (string title, string optional) {
    int width;
    string bar;

    if (!this_user()->query_account() || this_user()->query_account()->query_setting("screenreader") != "on") {
        width = this_user()->query_account() ? this_user()->query_account()->query_setting("width") : 80;
        bar = "%^RESET%^CYAN%^==";
        if (title) {
            bar += "%^RESET%^/ %^RESET%^BOLD%^" + title;
            if (optional) bar += ":%^RESET%^ " + optional;
            else bar += "%^RESET%^";
            bar += " \\%^CYAN%^=";
        }
        for (int i = strlen(strip_colour(bar)); i < width; i ++) bar += "=";
        bar += "%^RESET%^";
    } else {
        bar = title + (optional ? ": " + optional : "");
    }
    return bar;
}
string format_divider_bar () {
    int width;
    string bar = "";

    if (!this_user()->query_account() || this_user()->query_account()->query_setting("screenreader") != "on") {
        width = this_user()->query_account() ? this_user()->query_account()->query_setting("width") : 80;
        bar = "%^RESET%^BLUE%^-------------------------";
        for (int i = strlen(strip_colour(bar)); i < width; i ++) bar += "-";
        bar += "%^RESET%^";
    }
    return bar;
}
string format_footer_bar () {
    int width;
    string bar = "";

    if (!this_user()->query_account() || this_user()->query_account()->query_setting("screenreader") != "on") {
        width = this_user() ? this_user()->query_account()->query_setting("width") : 80;
        bar = "%^RESET%^CYAN%^=========================";
        for (int i = strlen(strip_colour(bar)); i < width; i ++) bar += "=";
        bar += "%^RESET%^";
    }
    return bar;
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
            case "enter": result += ({ "en" }); break;
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
            case "en": result += ({ "enter" }); break;
            default: result += ({ part });
        }
    }
    return implode(result, " ");
}