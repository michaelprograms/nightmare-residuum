// Converts two characters from base 16 to base 10
int hex_to_int (string b16) {
    int b10 = 0;

    if (!stringp(b16) || strlen(b16) < 2) error("Bad argument 1 to color->hex_to_int");
    switch (b16[0]) {
        case 'a'..'f':  b10 += (b16[0] - 'a' + 10) * 16;  break;
        case 'A'..'F':  b10 += (b16[0] - 'A' + 10) * 16;  break;
        case '0'..'9':  b10 += (b16[0] - '0') * 16;       break;
        default: error("Bad argument 1 to color->hex_to_int");
    }
    switch (b16[1]) {
        case 'a'..'f':  b10 += (b16[1] - 'a' + 10);   break;
        case 'A'..'F':  b10 += (b16[1] - 'A' + 10);   break;
        case '0'..'9':  b10 += (b16[1] - '0');        break;
        default: error("Bad argument 1 to color->hex_to_int");
    }
    return b10;
}

int *query_random_color () {
    int *c = ({ 0, 0, 0, });
    int r = 255;

    c[0] = 64 + random(192);
    r -= c[0];
    c[1] = random(r);
    r -= c[1];
    c[2] = random(r);

    c = shuffle(c);

    return c;
}