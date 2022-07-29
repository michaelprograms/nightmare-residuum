// Convert a string hex number to an int
int hex_to_int (string b16) {
    int b10;

    if (!stringp(b16) || !strlen(b16)) error("Bad argument 1 to color->hex_to_int");
    sscanf(b16, "%x", b10);
    if (undefinedp(b10)) error("Bad argument 1 to color->hex_to_int");

    return b10;
}

// Returns a random color triplet
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

// Returns a sRGB value in the range of [0..255] for linear input [0..1]
int color_to_sRGB (float n) {
    float f;

    if (!floatp(n) || undefinedp(n)) error("Bad argument 1 to color->color_to_sRGB");
    if (n <= 0.0031308) f = 12.92 * n;
    else f = (1.055 * pow(n, 1/2.4)) - 0.055;

    return to_int(255.9999 * f);
}

// Returns a linear value [0..1] for sRGB input [0..255]
float color_from_sRGB (int n) {
    float x, y;

    if (!intp(n) || undefinedp(n)) error("Bad argument 1 to color->color_from_sRGB");

    x = n / 255.0;
    if (x <= 0.04045) y = x / 12.92;
    else y = pow(((x + 0.055) / 1.055), 2.4);

    return to_float(sprintf("%0.2f", y));
}

// Linear Interpolation a color between two colors at a ratio
float color_lerp (float color1, float color2, float ratio) {
    if (!floatp(color1)) error("Bad argument 1 to color->color_lerp");
    if (!floatp(color2)) error("Bad argument 2 to color->color_lerp");
    if (undefinedp(ratio) || !floatp(ratio) || ratio < 0.0 || ratio > 1.0)  error("Bad argument 3 to color->color_lerp");

    return color1 * (1 - ratio) + color2 * ratio;
}

// Generate a color gradient between 'color1' and 'color2' of length 'steps'
string *color_gradient (int *color1, int *color2, int steps) {
    string *gradient = ({ });
    float *c1, *c2, *b;
    float ratio, intensity, *color, total;

    if (!arrayp(color1) && sizeof(color1) != 3) error("Bad argument 1 to color->color_gradient");
    if (!arrayp(color2) && sizeof(color2) != 3) error("Bad argument 2 to color->color_gradient");
    if (!intp(steps)) error("Bad argument 3 to color->color_gradient");

    c1 = ({ 0.0, 0.0, 0.0 });
    c2 = ({ 0.0, 0.0, 0.0 });
    b = ({ 0.0, 0.0 });

    // Red
    c1[0] = color_from_sRGB(color1[0]);
    c2[0] = color_from_sRGB(color2[0]);
    // Green
    c1[1] = color_from_sRGB(color1[1]);
    c2[1] = color_from_sRGB(color2[1]);
    // Blue
    c1[2] = color_from_sRGB(color1[2]);
    c2[2] = color_from_sRGB(color2[2]);
    // Bright
    b[0] = pow(c1[0] + c1[1] + c1[2], 0.43);
    b[1] = pow(c2[0] + c2[1] + c2[2], 0.43);

    for (int i = 0; i < steps; i ++) {
        ratio = i * 1.0 / (steps-1);
        intensity = color_lerp(b[0], b[1], ratio);
        color = ({
            color_lerp(c1[0], c2[0], ratio),
            color_lerp(c1[1], c2[1], ratio),
            color_lerp(c1[2], c2[2], ratio),
        });
        total = color[0] + color[1] + color[2];
        if (total != 0) {
            color[0] = color[0] * intensity / total;
            color[1] = color[1] * intensity / total;
            color[2] = color[2] * intensity / total;
        }
        // c[0] = color_to_sRGB(c[0]);
        // c[1] = color_to_sRGB(c[1]);
        // c[2] = color_to_sRGB(c[2]);
        color = map(color, (: color_to_sRGB :));
        gradient += ({ implode(map(color, (: ""+to_int($1) :)), ";") });
        // gradient += ({ implode(map(c, (: ""+to_int($1) :)), ";") });
        // gradient += ({ implode(map(color, (: ""+to_int(color_to_sRGB($1)) :)), ";") });
    }

    return gradient;
}