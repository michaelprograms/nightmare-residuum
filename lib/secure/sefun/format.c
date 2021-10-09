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
        bar = title + ": " + optional;
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