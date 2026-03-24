/**
 * Strip a leading indefinite or definite article from a string.
 *
 * @param str the string to strip
 * @returns the string without a leading "a", "an", or "the"
 */
string remove_article (string str) {
    string *matches;
    if (sizeof(matches = pcre_extract(str, "^(?:an?|the) (.+)$")) > 0) {
        str = matches[0];
    }
    return str;
}

/**
 * Prepend an article to a string. Strips any existing leading article first.
 * Defaults to an indefinite article ("a" or "an") based on the first character.
 *
 * @param str the string to prepend an article to
 * @param definite when non-zero, prepend "the" instead of "a"/"an"
 * @returns the string with an article prepended, or an empty string if str is empty
 */
varargs string add_article (string str, int definite) {
    if (!stringp(str) || !sizeof(str)) {
        str = "";
    } else {
        if (definite) {
            str = "the " + remove_article(str);
        } else {
            str = remove_article(str);
            if (member_array(str[0], ({ 'a','A','e','E','i','I','o','O','u','U' })) > -1) {
                str = "an " + str;
            } else {
                str = "a " + str;
            }
        }
    }
    return str;
}

/**
 * Return the cardinal word for an integer (one, two, three, etc).
 * Negative numbers are prefixed with "negative".
 *
 * @param n the integer to convert
 * @returns the cardinal word for n
 */
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

/**
 * Word-form ordinals for integers 0..9, keyed by integer value.
 */
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

/**
 * Return the ordinal form of a non-negative integer (zeroth, first, 2nd, 11th, 21st, etc).
 * Numbers 0..9 return word form. 11, 12, 13 and their higher-order equivalents
 * (111, 112, 113, 211, etc) always use "th".
 *
 * @param n the non-negative integer to convert
 * @returns the ordinal string for n
 */
string ordinal (int n) {
    int x;
    if (undefinedp(n) || !intp(n) || n < 0) {
        error("Bad argument 1 to grammar->ordinal");
    }
    if (n < 10) {
        return __Ordinals[n];
    }
    if (n % 100 >= 11 && n % 100 <= 13) {
        return n + "th";
    }
    x = n % 10;
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

/**
 * Irregular pluralization overrides. Maps a singular form to its correct plural.
 * Also used for words that are already plural (boots, gloves, etc).
 */
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

/**
 * Return the plural form of a word or object name. Handles ANSI color codes,
 * irregular overrides, and common English pluralization rules before falling
 * back to the driver efun.
 *
 * @param single the string or object to pluralize
 * @returns the plural form of the string
 */
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
        return __AbnormalOverride[str] + ansiTail;
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

/**
 * Format a count and noun together, pluralizing the noun when n != 1 and
 * stripping any leading article. Preserves parenthetical tags at the end
 * of the string (ex: "(wielded)", "(worn)").
 *
 * @param n the quantity
 * @param str the noun string, optionally prefixed with an article
 * @returns the consolidated string, ex: "three swords" or "a sword" when n == 1
 */
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

/**
 * Join a list of strings with a conjunction word. Uses Oxford comma style
 * for lists of three or more items.
 *
 * @param list the array of strings to join
 * @param conjunction the joining word; defaults to "and"
 * @returns the joined string, ex: "a, b, and c"
 */
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

/**
 * Return the possessive noun form of a name or object (ex: "Bob's", "Hermes'").
 * Returns "Its" when value is falsy or the object has no name.
 *
 * @param value a string name or object with query_cap_name()
 * @returns the possessive noun string
 */
string possessive_noun (mixed value) {
    if (!value) {
        return "Its";
    }
    if (objectp(value)) {
        value = /** @type {STD_OBJECT} value */(value)->query_cap_name();
        if (!value) {
            return "Its";
        }
    } else if (!stringp(value)) {
        error("Bad argument 1 to grammar->possessive_noun");
    }
    switch (value[<1]) {
        case 'z': case 's':
            return sprintf("%s'", value);
        default:
            return sprintf("%s's", value);
    }
}

/**
 * Return the subjective pronoun for a gender string or living object
 * (he, she, they, or it).
 *
 * @param value a gender string ("male", "female", "neither") or {STD_LIVING} object
 * @returns the subjective pronoun
 */
string subjective (mixed value) {
    string str;
    if (objectp(value)) {
        str = /** @type {STD_LIVING} value */(value)->query_gender();
    } else {
        str = value;
    }
    switch (str) {
        case "male":
            return "he";
        case "female":
            return "she";
        case "neither":
            return "they";
        default:
            return "it";
    }
}

/**
 * Return the objective pronoun for a gender string or living object
 * (him, her, them, or it).
 *
 * @param value a gender string ("male", "female", "neither") or {STD_LIVING} object
 * @returns the objective pronoun
 */
string objective (mixed value) {
    string str;
    if (objectp(value)) {
        str = /** @type {STD_LIVING} value */(value)->query_gender();
    } else {
        str = value;
    }
    switch (str) {
        case "male":
            return "him";
        case "female":
            return "her";
        case "neither":
            return "them";
        default:
            return "it";
    }
}

/**
 * Return the possessive pronoun for a gender string or living object
 * (his, her, their, or its).
 *
 * @param value a gender string ("male", "female", "neither") or {STD_LIVING} object
 * @returns the possessive pronoun
 */
string possessive (mixed value) {
    string str;
    if (objectp(value)) {
        str = /** @type {STD_LIVING} value */(value)->query_gender();
    } else {
        str = value;
    }
    switch (str) {
        case "male":
            return "his";
        case "female":
            return "her";
        case "neither":
            return "their";
        default:
            return "its";
    }
}

/**
 * Return the reflexive pronoun for a gender string or living object
 * (himself, herself, themself, or itself).
 *
 * @param value a gender string ("male", "female", "neither") or {STD_LIVING} object
 * @returns the reflexive pronoun
 */
string reflexive (mixed value) {
    return objective(value) + "self";
}
