#define ANSI(p)   "\e["+(p)+"m"
#define RESET     ANSI(0)
#define B_RED     ANSI(41)
#define B_GREEN   ANSI(42)
#define B_ORANGE  ANSI(43)

/**
 * Format the diff between actual (received) and expect (expected) as two
 * labeled lines. The `-` line shows the expected value; the `+` line shows
 * the received value. The shared prefix is highlighted with a GREEN
 * background; the expected divergence with ORANGE; the received divergence
 * with RED.
 *
 * @param actual the received value
 * @param expect the expected value
 * @returns two-line string: "    - '<expected>'\n    + '<actual>'"
 */
string format_string_difference (mixed actual, mixed expect) {
    int n;
    string shared;

    if (!stringp(actual)) {
        actual = identify(actual);
    }
    if (!stringp(expect)) {
        expect = identify(expect);
    }

    actual = replace_string(replace_string(replace_string(actual, "\n", "\\n"), "\e", "\\e"), "%^", "%%^%^^");
    expect = replace_string(replace_string(replace_string(expect, "\n", "\\n"), "\e", "\\e"), "%^", "%%^%^^");
    n = string_compare_same_until(actual, expect);
    shared = n ? B_GREEN + actual[0..n-1] + RESET : "";
    return "    - '" + shared + B_ORANGE + expect[n..] + RESET + "'\n" +
        "    + '" + shared + B_RED + actual[n..] + RESET + "'";
}

/**
 * Format per-index diffs between two arrays. Each index produces two lines:
 * a `-` line for the expected value and a `+` line for the received value.
 *
 * @param actual the received array
 * @param expect the expected array
 * @returns multi-line string, one pair of lines per index
 */
varargs string format_array_differences (mixed *actual, mixed *expect) {
    string result = "", a, e, shared;
    int i, l, n;

    l = max(({ sizeof(actual), sizeof(expect) }));

    for (i = 0; i < l; i ++) {
        if (i < sizeof(actual)) {
            if (arrayp(actual[i])) {
                a = implode(map(actual[i], (: identify($1) :)), ",");
            } else {
                a = actual[i];
            }
        } else {
            a = "";
        }
        if (i < sizeof(expect)) {
            if (arrayp(expect[i])) {
                e = implode(map(expect[i], (: identify($1) :)), ",");
            } else {
                e = expect[i];
            }
        } else {
            e = "";
        }

        if (!stringp(a)) {
            a = identify(a);
        }
        if (!stringp(e)) {
            e = identify(e);
        }
        a = replace_string(replace_string(replace_string(a, "\n", "\\n"), "\e", "\\e"), "%^", "%%^%^^");
        e = replace_string(replace_string(replace_string(e, "\n", "\\n"), "\e", "\\e"), "%^", "%%^%^^");
        n = string_compare_same_until(a, e);
        shared = n ? B_GREEN + a[0..n-1] + RESET : "";
        result += "\n      " + sprintf("%2d", i) + ". - '" + shared + B_ORANGE + e[n..] + RESET + "'" +
            "\n          + '" + shared + B_RED + a[n..] + RESET + "'";
    }
    return result;
}
