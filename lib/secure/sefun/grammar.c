string remove_article (string str) {
    string tmp;
    if (!stringp(str)) error("Bad argument 1 to grammar->remove_article");
    else if (sscanf(str, "the %s", tmp)) str = tmp;
    else if (sscanf(str, "a %s", tmp)) str = tmp;
    else if (sscanf(str, "an %s", tmp)) str = tmp;
    return str;
}

string cardinal (int n) {
    string sign;
    if (undefinedp(n) || !intp(n)) error("Bad argument 1 to grammar->cardinal");
    sign = (n < 0 ? "negative " : "");
    return sign + query_num(abs(n));
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

    if (n == 1 || !sizeof(str) ) return str;

    words = explode(str, " ");
    tmp = lower_case(words[0]); // @TODO strip_colours?
    if (member_array(tmp, ({"a","an","the","one"})) > -1) words = words[1..];
    result = sprintf("%s %s", cardinal(n), pluralize(implode(words, " ")));

    return result;
}

string conjunction (string *list) {
    string result = "";
    int i, max;

    if (arrayp(list)) {
        list = filter_array(list, (: stringp($1) && strlen($1) > 0 :));
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
varargs string possessive_noun (mixed value) {
    if (!value) {
        return "Its";
    }
    if (objectp(value)) {
        value = value->query_cap_name();
        if (!value) {
            value = "It";
        }
    } else if (!stringp(value)) {
        error("Bad argument 1 to possessive_noun().\n");
    }
    switch (value[<1]) {
    case 'x': case 'z': case 's': return sprintf("%s'", value);
    default: return sprintf("%s's", value);
    }
}

// subjective pronoun of an object
varargs string subjective (mixed value) {
    switch (objectp(value) ? value->query_gender() : value) {
    case "male": return "he";
    case "female": return "she";
    case "neither": return "they";
    default: return "it";
    }
}

// objective pronoun of an object
varargs string objective (mixed value) {
    switch (objectp(value) ? value->query_gender() : value) {
    case "male": return "him";
    case "female": return "her";
    case "neither": return "them";
    default: return "it";
    }
}

// possessive pronoun of an object
varargs string possessive (mixed value) {
    switch (objectp(value) ? value->query_gender() : value) {
    case "male": return "his";
    case "female": return "her";
    case "neither": return "their";
    default: return "its";
    }
}

// reflexive pronoun of an object
varargs string reflexive (mixed value) {
    return objective(value) + "self";
}