string remove_article (string str) {
    string tmp;
    if (!stringp(str)) {
        str = "";
    } else if (sscanf(str, "the %s", tmp)) {
        str = tmp;
    } else if (sscanf(str, "a %s", tmp)) {
        str = tmp;
    } else if (sscanf(str, "an %s", tmp)) {
        str = tmp;
    }
    return str;
}
varargs string add_article (string str, int definite) {
    if (!stringp(str) || !sizeof(str)) {
        str = "";
    } else {
        if (definite) {
            str = "the " + remove_article(str);
        } else if (member_array(str[0], ({ 'a','A','e','E','i','I','o','O','u','U' })) > -1) {
            str = "an " + str;
        } else {
            str = "a " + str;
        }
    }
    return str;
}

// a number denoting quantity (one, two, three, etc)
string cardinal (int n) {
    string sign;
    if (undefinedp(n) || !intp(n)) error("Bad argument 1 to grammar->cardinal");
    sign = (n < 0 ? "negative " : "");
    return sign + query_num(abs(n));
}

// returns a number indicating position or order (1st, 2nd, 3rd, tenth, etc)
string ordinal (int n) {
    int x;
    if (undefinedp(n) || !intp(n)) error("Bad argument 1 to grammar->ordinal");
    if (n < 0) error("Bad argument 1 to grammar->ordinal");
    if (n < 10) {
        if (n == 9) return "ninth";
        if (n == 8) return "eighth";
        if (n == 7) return "seventh";
        if (n == 6) return "sixth";
        if (n == 5) return "fifth";
        if (n == 4) return "fourth";
        if (n == 3) return "third";
        if (n == 2) return "second";
        if (n == 1) return "first";
        if (n == 0) return "zeroth";
    } else {
        x = (n < 14 && n > 10) ? 4 : n % 10;
        if (x == 1) return n + "st";
        else if (x == 2) return n + "nd";
        else if (x == 3) return n + "rd";
        else return n + "th";
    }
}

nosave private mapping __AbnormalOverride = ([
    "aren't": "isn't",
    "don't": "doesn't",
    "were": "was",
    "die": "dies",
    "boots": "boots",
    "bracers": "bracers",
    "gloves": "gloves",
    "leggings": "leggings",
    "robes": "robes",
    "shoes": "shoes",
]);
string pluralize (mixed single) {
    string str, last;

    if (objectp(single)) str = single->query_name();
    else str = single;
    if (!stringp(str)) error("Bad argument 1 to grammar->pluralize");

    if (__AbnormalOverride[str]) return __AbnormalOverride[str];
    last = explode(str, " ")[<1];
    if (__AbnormalOverride[last]) {
        return str[0..<(sizeof(last)+1)] + __AbnormalOverride[last];
    }

    if (str[<3..<1] == "uns") return str;
    if (str[<5..<1] == "staff") return str[0..<3] + "ves";
    else if (str[<2..<1] == "ff") return str + "s";
    return efun::pluralize(str);
}

string consolidate (int n, string str) {
    string *words;
    string tmp, result;
    string *tag;

    if (n == 1 || !sizeof(str)) {
        return str;
    }

    // store and remove tag
    tag = pcre_extract(str, "(\\(.+\\))$");
    if (sizeof(tag)) {
        str = replace_string(str, tag[0], "");
    }

    words = explode(str, " ");
    tmp = lower_case(words[0]);
    if (member_array(tmp, ({"a","an","the","one"})) > -1) words = words[1..];

    result = sprintf("%s %s", cardinal(n), pluralize(implode(words, " ")));

    // restore tag
    if (sizeof(tag)) {
        result += " " + tag[0];
    }

    return result;
}

string conjunction (string *list) {
    string result = "";
    int i, max;

    if (arrayp(list)) {
        list = filter(list, (: stringp($1) && strlen($1) > 0 :));
    }
    if (!arrayp(list) || !sizeof(list)) error("Bad argument 1 to grammar->conjunction");
    for (i = 0, max = sizeof(list); i < max; i ++) {
        if (i == max - 1 && max > 1) result += "and ";
        result += list[i];
        if (i == max - 1) continue;
        else if (max > 2) result += ", ";
        else result += " ";
    }
    return result;
}

// named possessive of an object
string possessive_noun (mixed value) {
    if (!value) {
        return "Its";
    }
    if (objectp(value)) {
        value = value->query_cap_name();
        if (!value) {
            value = "It";
        }
    } else if (!stringp(value)) {
        error("Bad argument 1 to grammar->possessive_noun\n");
    }
    switch (value[<1]) {
    case 'z': case 's': return sprintf("%s'", value);
    default: return sprintf("%s's", value);
    }
}

// subjective pronoun of an object
string subjective (mixed value) {
    switch (objectp(value) ? value->query_gender() : value) {
    case "male": return "he";
    case "female": return "she";
    case "neither": return "they";
    default: return "it";
    }
}

// objective pronoun of an object
string objective (mixed value) {
    switch (objectp(value) ? value->query_gender() : value) {
    case "male": return "him";
    case "female": return "her";
    case "neither": return "them";
    default: return "it";
    }
}

// possessive pronoun of an object
string possessive (mixed value) {
    switch (objectp(value) ? value->query_gender() : value) {
    case "male": return "his";
    case "female": return "her";
    case "neither": return "their";
    default: return "its";
    }
}

// reflexive pronoun of an object
string reflexive (mixed value) {
    return objective(value) + "self";
}