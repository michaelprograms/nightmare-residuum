nosave private mixed __Long;
nosave private mixed __Short;

nomask void set_long (mixed m) {
    __Long = m;
}
string query_long () {
    return evaluate(__Long);
}

nomask void set_short (mixed m) {
    __Short = m;
}
varargs string query_short (string defaultANSI) {
    string s = evaluate(__Short);

    // handle %^DEFAULT%^ in shorts
    if (strlen(s) && regexp(s, "DEFAULT")) {
        if (undefinedp(defaultANSI)) defaultANSI = "%^RESET%^";
        s = replace_string(s, "%^DEFAULT%^", defaultANSI);
    }

    if (this_object()->query_defeated()) {
        s += " ((%^WHITE%^defeated%^DEFAULT%^))";
    }

    return s;
}