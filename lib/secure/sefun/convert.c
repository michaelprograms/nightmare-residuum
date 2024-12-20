/**
 * Convert a string hex base16 number to an integer base10 number.
 *
 * @param b16 hex base16 number to convert
 * @returns base10 integer number
 */
int hex_to_int (string b16) {
    int b10;

    if (!stringp(b16) || !strlen(b16)) {
        error("Bad argument 1 to convert->hex_to_int");
    }
    sscanf(b16, "%x", b10);
    return b10;
}

/**
 * Convert an integer base10 to string base2
 *
 * @param b10 integer base10 number to convert
 * @returns string base2 number
 */
string int_to_binary (int b10) {
    string b2 = "";

    for (int i = 63; i >= 0; i --) {
        // Check if the ith bit is set
        b2 += b10 & (1 << i) ? "1" : "0";
    }
    b2 = pcre_replace(b2, "^(0*)", ({ "" }));
    if (b2 == "") {
        b2 = "0";
    }
    return b2;
}