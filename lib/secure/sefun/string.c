// This sefun is called by efun::terminal_colours
string strip_colour (string str) {
    // parse blank ANSI color tags
    str = terminal_colour(str, D_ANSI->query_unknown_term());
    // strip default ANSI reset color tag added by fluffos
    str = regreplace(str, "\e[49;49m", "", 1);
    str = regreplace(str, "\e[0;10m", "", 1);
    return str;
}

string identify (mixed a) {
    int i, s;
    string ret;
    mapping RealMap;

    // @LDMUD has no UNDEFINED
    // if (undefinedp(a)) {
    //     return "UNDEFINED";
    // }
    if (intp(a) || floatp(a)) {
        return "" + a;
    }
    if (objectp(a)) {
        if (ret = a->query_key_name()) ret += " ";
        else ret = "";
        return "OBJ(" + ret + program_name(a) + ")";
    }
    if (stringp(a)) {
        a = regreplace(a, "\"", "\\\"", 1);
        a = "\"" + a + "\"";
        // a = regreplace(a, "\\", "\\\\", 1);
        a = regreplace(a, "\\\"", "\"", 1);
        a = regreplace(a, "\n", "\\n", 1);
        a = regreplace(a, "\t", "\\t", 1);
        return a;
    }
    if (mappingp(a)) {
        ret = "([ ";
        RealMap = a;
        a = sort_array(m_indices(RealMap), (: $1 > $2 :));
        s = sizeof(a);
        for (i = 0 ; i < s ; i ++) {
            if (i) ret += ", ";
            ret += identify(a[i]) + ": " + identify(RealMap[a[i]]);
        }
        return ret + (s ? " " : "") + "])";
    }
    if (closurep(a)) {
        return sprintf("%O", a);
    }
    // @LDMUD has no class type
    // if (classp(a)) {
    //     return regreplace(sprintf("%O", a), "\n", "", 1);
    // }
    if (pointerp(a)) {
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
    if (!width || width < 0) {
        if (po && po->query_account()) width = po->query_account()->query_setting("width");
        else width = 80;
    }

    if (po && po->query_account() && po->query_account()->query_setting("ansi") == "on") {
        term = D_ANSI->query_ansi_term();
    } else {
        term = D_ANSI->query_unknown_term();
    }
    str = terminal_colour(str, term, width, indent);
    debug_message("wrap: "+str+" "+width+" "+indent+"\n");
    // str = regreplace(str, "\e[49;49m", "", 1);
    // str = regreplace(str, "\e[0;10m", "", 1);
    if (sizeof(str) > __LARGEST_PRINTABLE_STRING__) {
        str = str[0..__LARGEST_PRINTABLE_STRING__ - 1];
    }
    return str;
}

int string_compare_same_until (string a, string b) {
    // Returns n where a[0..n] == b[0..n]
    int n = 0, l;

    if ((l = sizeof(a)) == sizeof(b) && a == b) return l;
    if (sizeof(a) > sizeof(b)) l = sizeof(b);

    for (int i = 0; i < l; i ++) {
        if (a[i] != b[i]) break;
        n ++;
    }
    return n;
}

string sanitize_name (string name) {
    if (!stringp(name)) raise_error("Bad argument 1 to string->sanitize_name");
    name = regreplace(name, " ", "", 1);
    name = regreplace(name, "'", "", 1);
    name = regreplace(name, "-", "", 1);
    return lower_case(name);
}