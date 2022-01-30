// This sefun is called by efun::terminal_colours
string strip_colour (string str) {
    // parse blank ANSI color tags
    str = terminal_colour(str, D_ANSI->query_unknown_term());
    // strip default ANSI reset color tag added by fluffos
    str = replace_string(str, "\e[49;49m", "");
    str = replace_string(str, "\e[0;10m", "");
    return str;
}

varargs string center (string str, int width) {
    int len, pad;
    if (!str || str == "") return "";
    len = strlen(strip_colour(str));
    if (!width) width = 80;
    else if (width < len) return str;
    pad = (width - len) / 2;
    return sprintf("%"+sprintf("%d", pad)+"s", " ") + str + sprintf("%"+sprintf("%d", width - len - pad)+"s", " ");
}
varargs string pad_left (string text, int n, string pad) {
    if (n < 0 || n > 80) return text;
    if (undefinedp(pad)) pad = " ";
    while (sizeof(text) < n) text = pad + text;
    return text;
}
varargs string pad_right (string text, int n, string pad) {
    if (n < 0 || n > 80) return text;
    if (undefinedp(pad)) pad = " ";
    while (sizeof(text) < n) text = text + pad;
    return text;
}

varargs string identify (mixed a) {
    int i, s;
    string ret;
    mapping RealMap;

    if (undefinedp(a)) {
        return "UNDEFINED";
    }
    if (intp(a) || floatp(a)) {
        return "" + a;
    }
    if (objectp(a)) {
        if (ret = a->query_key_name()) ret += " ";
        else ret = "";
        return "OBJ(" + ret + file_name(a) + ")";
    }
    if (stringp(a)) {
        a = replace_string(a, "\"", "\\\"");
        a = "\"" + a + "\"";
        a = replace_string(a, "\\", "\\\\");
        a = replace_string(a, "\\\"", "\"");
        a = replace_string(a, "\n", "\\n");
        a = replace_string(a, "\t", "\\t");
        return a;
    }
    if (mapp(a)) {
        ret = "([ ";
        RealMap = a;
        a = sort_array(keys(RealMap), 1);
        s = sizeof(a);
        for (i = 0 ; i < s ; i ++) {
            if (i) ret += ", ";
            ret += identify(a[i]) + ": " + identify(RealMap[a[i]]);
        }
        return ret + (s ? " " : "") + "])";
    }
    if (functionp(a)) {
        return sprintf("%O", a);
    }
    if (classp(a)) {
        return replace_string(sprintf("%O", a), "\n", "");
    }
    if (arrayp(a)) {
        ret = "({ ";
        s = sizeof(a);
        for (i = 0; i < s; i ++) {
            if (i) ret += ", ";
            ret += identify(a[i]);
        }
        return ret + (s ? " " : "") + "})";
    }
    return "UNKNOWN";
}

varargs string wrap (string str, int width, int indent) {
    mapping term;
    object po = previous_object();

    if (!str || !stringp(str)) return "";
    if (undefinedp(width) || width < 0) {
        if (po && po->query_account()) width = po->query_account()->query_setting("width");
        else width = 80;
    }

    if (po && po->query_account() && po->query_account()->query_setting("ansi") == "on") {
        term = D_ANSI->query_ansi_term();
    } else {
        term = D_ANSI->query_unknown_term();
    }
    str = terminal_colour(str, term, width, indent);
    str = replace_string(str, "\e[49;49m", "");
    str = replace_string(str, "\e[0;10m", "");
    if (strlen(str) > __LARGEST_PRINTABLE_STRING__) {
        str = str[0..__LARGEST_PRINTABLE_STRING__ - 1];
    }
    return str;
}

int string_compare_same_until (string a, string b) {
    // Returns n where a[0..n] == b[0..n]
    int n = 0, l;

    if ((l = strlen(a)) == strlen(b) && a == b) return l;
    if (strlen(a) > strlen(b)) l = strlen(b);

    for (int i = 0; i < l; i ++) {
        if (a[i] != b[i]) break;
        n ++;
    }
    return n;
}

string sanitize_name (string name) {
    if (undefinedp(name) || !stringp(name)) error("Bad argument 1 to string->sanitize_name");
    name = replace_string(name, " ", "");
    name = replace_string(name, "'", "");
    name = replace_string(name, "-", "");
    return lower_case(name);
}