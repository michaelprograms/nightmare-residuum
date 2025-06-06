string remove_article (string str) {
    string *matches;
    if (sizeof(matches = pcre_extract(str, "^(?:an?|the) (.+)$")) > 0) {
        str = matches[0];
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
    string sign = "";
    if (undefinedp(n) || !intp(n)) {
        error("Bad argument 1 to grammar->cardinal");
    }
    if (n < 0) {
        sign = "negative ";
    }
    return sign + query_num(abs(n));
}

nosave private mapping __Ordinals = ([
    9: "ninth",
    8: "eighth",
    7: "seventh",
    6: "sixth",
    5: "fifth",
    4: "fourth",
    3: "third",
    2: "second",
    1: "first",
    0: "zeroth",
]);
// returns a number indicating position or order (1st, 2nd, 3rd, tenth, etc)
string ordinal (int n) {
    int x;
    if (undefinedp(n) || !intp(n) || n < 0) {
        error("Bad argument 1 to grammar->ordinal");
    }
    if (n < 10) {
        return __Ordinals[n];
    } else {
        x = (n < 14 && n > 10) ? 4 : n % 10;
        if (x == 1) {
            return n + "st";
        } else if (x == 2) {
            return n + "nd";
        } else if (x == 3) {
            return n + "rd";
        } else {
            return n + "th";
        }
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
    string ansiTail = "";
    int n;

    if (objectp(single)) {
        str = /** @type {STD_OBJECT} single */(single)->query_name();
    } else {
        str = single;
    }
    if (!stringp(str)) {
        error("Bad argument 1 to grammar->pluralize");
    }

    if (str[<2..<1] == "%^") {
        n = strsrch(str[0..<3], "%^", -1);
        ansiTail = str[n..];
        str = str[0..n-1];
    }
    if (__AbnormalOverride[str]) {
        return __AbnormalOverride[str];
    }
    last = explode(str, " ")[<1];
    if (__AbnormalOverride[last]) {
        return str[0..<(sizeof(last)+1)] + __AbnormalOverride[last] + ansiTail;
    }

    if (str[<3..<1] == "uns") {
        return str + ansiTail;
    } else if (str[<5..<1] == "staff") {
        return str[0..<3] + "ves" + ansiTail;
    } else if (str[<2..<1] == "ff") {
        return str + "s" + ansiTail;
    } else {
        return efun::pluralize(str) + ansiTail;
    }
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
    if (member_array(tmp, ({"a","an","the","one"})) > -1) {
        words = words[1..];
    }

    result = sprintf("%s %s", cardinal(n), pluralize(implode(words, " ")));

    // restore tag
    if (sizeof(tag)) {
        result += " " + tag[0];
    }

    return result;
}

varargs string conjunction (string *list, string conjunction) {
    string result = "";
    int l;

    if (!arrayp(list) || !sizeof(list)) {
        error("Bad argument 1 to grammar->conjunction");
    }
    if (undefinedp(conjunction)) {
        conjunction = "and";
    }

    l = sizeof(list);
    if (l == 1) {
        result = list[0];
    } else if (l == 2) {
        result = list[0] + " " + conjunction + " " + list[1];
    } else if (l > 1) {
        list[<1] = conjunction + " " + list[<1];
        result = implode(list, ", ");
    }
    return result;
}

// named possessive of an object
string possessive_noun (mixed value) {
    if (!value) {
        return "Its";
    }
    if (objectp(value)) {
        value = /** @type {STD_OBJECT} value */(value)->query_cap_name();
        if (!value) {
            value = "It";
        }
    } else if (!stringp(value)) {
        error("Bad argument 1 to grammar->possessive_noun");
    }
    switch (value[<1]) {
    case 'z': case 's': return sprintf("%s'", value);
    default: return sprintf("%s's", value);
    }
}

// subjective pronoun of an object
string subjective (mixed value) {
    string str;
    if (objectp(value)) {
        str = /** @type {STD_LIVING} value */(value)->query_gender();
    } else {
        str = value;
    }
    switch (str) {
        case "male": return "he";
        case "female": return "she";
        case "neither": return "they";
        default: return "it";
    }
}

// objective pronoun of an object
string objective (mixed value) {
    string str;
    if (objectp(value)) {
        str = /** @type {STD_LIVING} value */(value)->query_gender();
    } else {
        str = value;
    }
    switch (str) {
        case "male": return "him";
        case "female": return "her";
        case "neither": return "them";
        default: return "it";
    }
}

// possessive pronoun of an object
string possessive (mixed value) {
    string str;
    if (objectp(value)) {
        str = /** @type {STD_LIVING} value */(value)->query_gender();
    } else {
        str = value;
    }
    switch (str) {
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