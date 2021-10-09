// Converts two characters from base 16 to base 10
int hex_to_int (string base16) {
    int base10 = 0;

    if (undefinedp(base16) || !stringp(base16) || strlen(base16) < 2) error("Bad argument 1 to color->hex_to_int");
    switch (base16[0]) {
        case 'a'..'f':  base10 += (base16[0] - 'a' + 10) * 16;  break;
        case 'A'..'F':  base10 += (base16[0] - 'A' + 10) * 16;  break;
        case '0'..'9':  base10 += (base16[0] - '0') * 16;       break;
        default: error("Bad argument 1 to color->hex_to_int");
    }
    switch (base16[1]) {
        case 'a'..'f':  base10 += (base16[1] - 'a' + 10);   break;
        case 'A'..'F':  base10 += (base16[1] - 'A' + 10);   break;
        case '0'..'9':  base10 += (base16[1] - '0');        break;
        default: error("Bad argument 1 to color->hex_to_int");
    }
    return base10;
}