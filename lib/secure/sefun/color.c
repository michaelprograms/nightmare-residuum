int hex_to_int(string b16) {
    int b10;

    if (!stringp(b16) || !strlen(b16)) error("Bad argument 1 to color->hex_to_int");
    sscanf(b16, "%x", b10);
    if (undefinedp(b10)) error("Bad argument 1 to color->hex_to_int");

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