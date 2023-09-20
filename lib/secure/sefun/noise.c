// Perlin noise algorithm for generating seeded randomized values
// https://rosettacode.org/wiki/Perlin_noise

// Ken Perlin's hash lookup table, a randomly arranged array from [0-255]
#define PERMUTATION ({ \
151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225,\
140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148,\
247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32, \
57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175,\
74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,\
60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54, \
65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,\
200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64, \
52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212,\
207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213,\
119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,  \
129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104,\
218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241,\
81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,\
184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93, \
222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180,\
})

/* ----- perlin seeded permutation ----- */

// uses Xorshift128+ to generate a seeded permutation array
// https://stackoverflow.com/questions/34426499/what-is-the-real-definition-of-the-xorshift128-algorithm
int *noise_generate_permutation (string seed) {
    int i, j, swap;
    mixed *p;
    int state0 = 0, state1 = 0, s0, s1;

    if (!stringp(seed) || !sizeof(seed)) {
        return PERMUTATION;
    }

    // Xorshift128+ PRNG initialize: turn seed into positive integer in state0
    p = map(explode(seed, ""), (: $1[0] :));
    j = sizeof(p);
    for (i = 0; i < j; i ++) {
        state0 += p[i];
    }
    state0 &= 0x7fffffff;
    state1 = state0 + 0x9E3779B9;

    // set default permutation
    p = PERMUTATION;

    for (i = 0; i <= 255; i ++) {
        // Xorshift128+ PRNG generate next integer
        s1 = state0;
        s0 = state1;
        state0 = s0;
        s1 ^= s1 << 23;
        s1 ^= s1 >> 18;
        s1 ^= s0;
        s1 ^= s0 >> 5;
        state1 = s1;
        j = state1 % (255 - i + 1);

        // swap with random remaining permutation index
        swap   = p[i  ];
        p[i  ] = p[i+j];
        p[i+j] = swap;
    }

    return p;
}

// include precalculated 4d gradient x,y,z,w arrays for optimization
mapping noise_generate_permutation_simplex (string seed) {
    int *GRAD4 = ({
        0,  1,  1,  1,  0,  1,  1,  -1,  0,  1,  -1, 1,  0,  1,  -1, -1,
        0,  -1, 1,  1,  0,  -1, 1,  -1,  0,  -1, -1, 1,  0,  -1, -1, -1,
        1,  0,  1,  1,  1,  0,  1,  -1,  1,  0,  -1, 1,  1,  0,  -1, -1,
        -1, 0,  1,  1,  -1, 0,  1,  -1,  -1, 0,  -1, 1,  -1, 0,  -1, -1,
        1,  1,  0,  1,  1,  1,  0,  -1,  1,  -1, 0,  1,  1,  -1, 0,  -1,
        -1, 1,  0,  1,  -1, 1,  0,  -1,  -1, -1, 0,  1,  -1, -1, 0,  -1,
        1,  1,  1,  0,  1,  1,  -1,  0,  1,  -1, 1,  0,  1,  -1, -1, 0,
        -1, 1,  1,  0,  -1, 1,  -1,  0,  -1, -1, 1,  0,  -1, -1, -1, 0,
    });
    mapping p = ([
        "p": noise_generate_permutation(seed),
    ]);
    p["x"] = map(p["p"], (: $(GRAD4)[($1 % 32) * 4] :));
    p["y"] = map(p["p"], (: $(GRAD4)[($1 % 32) * 4 + 1] :));
    p["z"] = map(p["p"], (: $(GRAD4)[($1 % 32) * 4 + 2] :));
    p["w"] = map(p["p"], (: $(GRAD4)[($1 % 32) * 4 + 3] :));
    return p;
}

/* ----- noise helper functions ----- */

// Fade eases & smooths coordinate values towards integral values
float noise_perlin_fade (float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
// Source: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
float noise_perlin_grad (int hash, float x, float y, float z) {
    switch(hash & 0xF) {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default:  return 0; // never happens
    }
}
// Linear interpolation: blend values a and b based on weight t [0.0-1.0]
float noise_perlin_lerp (float t, float a, float b) {
    if (0.0 > t) return a;
    if (1.0 < t) return b;
    return a + t * (b - a);
}

/* ----- noise permutation functions ----- */

float noise_perlin_2d_permutation (float x, float y, int *p) {
    int X, Y;
    float u, v;
    int A, A1, B, B1;

    // find unit square that contains point
    X = to_int(x) & 255;
    Y = to_int(y) & 255;

    // find relative x,y of point in square
    x -= floor(x);
    y -= floor(y);

    // compute fade curves for each of x,y
    u = noise_perlin_fade(x);
    v = noise_perlin_fade(y);

    // hash coordinates of the 4 corners
    // https://saturncloud.io/blog/producing-2d-perlin-noise-with-numpy-a-comprehensive-guide-for-data-scientists
    A  = (p[X          ] + Y) & 255;
    A1 = (A + 1             ) & 255;
    B  = (p[(X+1) & 255] + Y) & 255;
    B1 = (B + 1             ) & 255;

    // add blended results from the 4 corners
    return noise_perlin_lerp(
        v,
        noise_perlin_lerp(
            u,
            noise_perlin_grad(p[A ], x  , y  , 0),
            noise_perlin_grad(p[B ], x-1, y  , 0)
        ),
        noise_perlin_lerp(
            u,
            noise_perlin_grad(p[A1], x  , y-1, 0),
            noise_perlin_grad(p[B1], x-1, y-1, 0)
        )
    );
}
float noise_perlin_3d_permutation (float x, float y, float z, int *p) {
    int X, Y, Z;
    float u, v, w;
    int A, AA, AB, B, BA, BB;

    // find unit cube that contains point
    X = to_int(x) & 255;
    Y = to_int(y) & 255;
    Z = to_int(z) & 255;

    // find relative x,y,z of point in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // compute fade curves for each of x,y,z
    u = noise_perlin_fade(x);
    v = noise_perlin_fade(y);
    w = noise_perlin_fade(z);

    // hash coordinates of the 8 cube corners
    A  = (p[X          ] + Y) & 255;
    AA = (p[A          ] + Z) & 255;
    AB = (p[(A+1) & 255] + Z) & 255;
    B  = (p[(X+1) & 255] + Y) & 255;
    BA = (p[B          ] + Z) & 255;
    BB = (p[(B+1) & 255] + Z) & 255;

    // add blended results from 8 corners of cube
    return noise_perlin_lerp(
        w,
        noise_perlin_lerp(
            v,
            noise_perlin_lerp(
                u,
                noise_perlin_grad(p[AA        ], x  , y  , z  ),
                noise_perlin_grad(p[BA        ], x-1, y  , z  )
            ),
            noise_perlin_lerp(
                u,
                noise_perlin_grad(p[AB        ], x  , y-1, z  ),
                noise_perlin_grad(p[BB        ], x-1, y-1, z  )
            )
        ),
        noise_perlin_lerp(
            v,
            noise_perlin_lerp(
                u,
                noise_perlin_grad(p[AA+1 & 255], x  , y  , z-1),
                noise_perlin_grad(p[BA+1 & 255], x-1, y  , z-1)
            ),
            noise_perlin_lerp(
                u,
                noise_perlin_grad(p[AB+1 & 255], x  , y-1, z-1),
                noise_perlin_grad(p[BB+1 & 255], x-1, y-1, z-1)
            )
        )
    );
}

// @TODO noise_perlin_4d_permutatation (float x, float y, float z, float w, int *p)

/* ----- noise_perlin functions ----- */

// generate noise at coordinates x,y using a permutation, octaves, and scale
float noise_perlin_2d (float x, float y, int *p, int octaves, float scale) {
    float total = 0.0;
    float amplitude = 1.0, divisor = 0.0;

    if (!floatp(x)) {
        error("Bad argument 1 to noise->noise_perlin_2d");
    }
    if (!floatp(y)) {
        error("Bad argument 2 to noise->noise_perlin_2d");
    }
    if (!arrayp(p)) {
        error("Bad argument 3 to noise->noise_perlin_2d");
    }
    if (!octaves) {
        octaves = 4;
    }
    if (!scale) {
      scale = 1.0;
    }

    for (int i = 0; i < octaves; i ++) {
        total += noise_perlin_2d_permutation(x * scale, y * scale, p) * amplitude;
        divisor += amplitude;
        scale *= 2.0;
        amplitude *= 0.5;
    }

    divisor = 1.0;
    return total / (divisor ? divisor : 1.0);
}

// generate noise at coordinates x,y,z using a permutation, octaves, and scale
float perlin_noise_3d (float x, float y, float z, int *p, int octaves, float scale) {
    float total = 0.0;
    float amplitude = 1.0, divisor = 0.0;

    if (!floatp(x)) {
        error("Bad argument 1 to noise->perlin_noise_3d");
    }
    if (!floatp(y)) {
        error("Bad argument 2 to noise->perlin_noise_3d");
    }
    if (!floatp(z)) {
        error("Bad argument 3 to noise->perlin_noise_3d");
    }
    if (!arrayp(p)) {
        error("Bad argument 4 to noise->perlin_noise_3d");
    }
    if (!octaves) {
        octaves = 4;
    }
    if (!scale) {
      scale = 1.0;
    }

    for (int i = 0; i < octaves; i ++) {
        total += noise_perlin_3d_permutation(x * scale, y * scale, z * scale, p) * amplitude;
        divisor += amplitude;
        scale *= 2.0;
        amplitude *= 0.5;
    }

    divisor = 1.0;
    return total / (divisor ? divisor : 1.0);
}

/* ----- simplex permutation functions ----- */

// https://github.com/jwagner/simplex-noise.js
float noise_simplex_4d_permutation (float x, float y, float z, float w, mapping p) {
    float n0, n1, n2, n3, n4; // Noise contributions from the five corners

    // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
    float s = (x + y + z + w) * 0.309017; // Factor for 4D skewing: (sqrt(5.0)-1)/4

    int i = to_int(floor(x + s));
    int j = to_int(floor(y + s));
    int k = to_int(floor(z + s));
    int l = to_int(floor(w + s));

    float t = (i + j + k + l) * 0.138197; // Factor for 4D unskewing: (5-sqrt(5.0))/20
    float X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
    float Y0 = j - t;
    float Z0 = k - t;
    float W0 = l - t;

    float x0 = x - X0; // The x,y,z,w distances from the cell origin
    float y0 = y - Y0;
    float z0 = z - Z0;
    float w0 = w - W0;

    // For the 4D case, the simplex is a 4D shape I won't even try to describe.
    // To find out which of the 24 possible simplices we're in, we need to
    // determine the magnitude ordering of x0, y0, z0 and w0.
    // Six pair-wise comparisons are performed between each possible pair
    // of the four coordinates, and the results are used to rank the numbers.
    int rankx = 0;
    int ranky = 0;
    int rankz = 0;
    int rankw = 0;

    int i1, j1, k1, l1, i2, j2, k2, l2, i3, j3, k3, l3;
    float x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4;
    int ii, jj, kk, ll;
    float t0, t1, t2, t3, t4;
    int gi0, gi1, gi2, gi3, gi4;

    if (x0 > y0) rankx ++;
    else ranky ++;
    if (x0 > z0) rankx ++;
    else rankz ++;
    if (x0 > w0) rankx ++;
    else rankw ++;
    if (y0 > z0) ranky ++;
    else rankz ++;
    if (y0 > w0) ranky ++;
    else rankw ++;
    if (z0 > w0) rankz ++;
    else rankw ++;

    // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
    // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
    // impossible. Only the 24 indices which have non-zero entries make any sense.
    // We use a thresholding to set the coordinates in turn from the largest magnitude.
    // Rank 3 denotes the largest coordinate.
    // Rank 2 denotes the second largest coordinate.
    // Rank 1 denotes the second smallest coordinate.
    // The integer offsets for the second simplex corner
    i1 = rankx >= 3 ? 1 : 0;
    j1 = ranky >= 3 ? 1 : 0;
    k1 = rankz >= 3 ? 1 : 0;
    l1 = rankw >= 3 ? 1 : 0;
    // The integer offsets for the third simplex corner
    i2 = rankx >= 2 ? 1 : 0;
    j2 = ranky >= 2 ? 1 : 0;
    k2 = rankz >= 2 ? 1 : 0;
    l2 = rankw >= 2 ? 1 : 0;
    // The integer offsets for the fourth simplex corner
    i3 = rankx >= 1 ? 1 : 0;
    j3 = ranky >= 1 ? 1 : 0;
    k3 = rankz >= 1 ? 1 : 0;
    l3 = rankw >= 1 ? 1 : 0;

    // The fifth corner has all coordinate offsets = 1, so no need to compute that.
    x1 = x0 - i1 + 0.138197; // Offsets for second corner in (x,y,z,w) coords
    y1 = y0 - j1 + 0.138197;
    z1 = z0 - k1 + 0.138197;
    w1 = w0 - l1 + 0.138197;
    x2 = x0 - i2 + 2.0 * 0.138197; // Offsets for third corner in (x,y,z,w) coords
    y2 = y0 - j2 + 2.0 * 0.138197;
    z2 = z0 - k2 + 2.0 * 0.138197;
    w2 = w0 - l2 + 2.0 * 0.138197;
    x3 = x0 - i3 + 3.0 * 0.138197; // Offsets for fourth corner in (x,y,z,w) coords
    y3 = y0 - j3 + 3.0 * 0.138197;
    z3 = z0 - k3 + 3.0 * 0.138197;
    w3 = w0 - l3 + 3.0 * 0.138197;
    x4 = x0 - 1.0 + 4.0 * 0.138197; // Offsets for last corner in (x,y,z,w) coords
    y4 = y0 - 1.0 + 4.0 * 0.138197;
    z4 = z0 - 1.0 + 4.0 * 0.138197;
    w4 = w0 - 1.0 + 4.0 * 0.138197;

    // Work out the hashed gradient indices of the five simplex corners
    ii = i & 255;
    jj = j & 255;
    kk = k & 255;
    ll = l & 255;

    // Calculate the contribution from the five corners
    t0 = 0.6 - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
    if (t0 < 0) n0 = 0.0;
    else {
        gi0 = ii + p["p"][(jj + p["p"][(kk + p["p"][ll]) & 255]) & 255];
        t0 *= t0;
        n0 = t0 * t0 * (p["x"][gi0 & 255] * x0 + p["y"][gi0 & 255] * y0 + p["z"][gi0 & 255] * z0 + p["w"][gi0 & 255] * w0);
    }
    t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
    if (t1 < 0) n1 = 0.0;
    else {
        gi1 = ii + i1 + p["p"][(jj + j1 + p["p"][(kk + k1 + p["p"][(ll + l1) & 255]) & 255]) & 255];
        t1 *= t1;
        n1 = t1 * t1 * (p["x"][gi1 & 255] * x1 + p["y"][gi1 & 255] * y1 + p["z"][gi1 & 255] * z1 + p["w"][gi1 & 255] * w1);
    }
    t2 = 0.6 - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
    if (t2 < 0) n2 = 0.0;
    else {
        gi2 = ii + i2 + p["p"][(jj + j2 + p["p"][(kk + k2 + p["p"][(ll + l2) & 255]) & 255]) & 255];
        t2 *= t2;
        n2 = t2 * t2 * (p["x"][gi2 & 255] * x2 + p["y"][gi2 & 255] * y2 + p["z"][gi2 & 255] * z2 + p["w"][gi2 & 255] * w2);
    }
    t3 = 0.6 - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
    if (t3 < 0) n3 = 0.0;
    else {
        gi3 = ii + i3 + p["p"][(jj + j3 + p["p"][(kk + k3 + p["p"][(ll + l3) & 255]) & 255]) & 255];
        t3 *= t3;
        n3 = t3 * t3 * (p["x"][gi3 & 255] * x3 + p["y"][gi3 & 255] * y3 + p["z"][gi3 & 255] * z3 + p["w"][gi3 & 255] * w3);
    }
    t4 = 0.6 - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
    if (t4 < 0) n4 = 0.0;
    else {
        gi4 = ii + 1 + p["p"][(jj + 1 + p["p"][(kk + 1 + p["p"][(ll + 1) & 255]) & 255]) & 255];
        t4 *= t4;
        n4 = t4 * t4 * (p["x"][gi4 & 255] * x4 + p["y"][gi4 & 255] * y4 + p["z"][gi4 & 255] * z4 + p["w"][gi4 & 255] * w4);
    }
    // Sum up and scale the result to cover the range [-1,1]
    return 27.0 * (n0 + n1 + n2 + n3 + n4);
}
float noise_simplex_4d (float x, float y, float z, float w, mapping p, int octaves, float scale) {
    float total = 0.0;
    float t = 0.0;
    int f = 1;

    if (!floatp(x)) {
        error("Bad argument 1 to noise->noise_simplex_4d");
    }
    if (!floatp(y)) {
        error("Bad argument 2 to noise->noise_simplex_4d");
    }
    if (!floatp(z)) {
        error("Bad argument 3 to noise->noise_simplex_4d");
    }
    if (!floatp(w)) {
        error("Bad argument 4 to noise->noise_simplex_4d");
    }
    if (!mapp(p)) {
        error("Bad argument 5 to noise->noise_simplex_4d");
    }
    if (!octaves) {
        octaves = 4;
    }
    if (!scale) {
      scale = 1.0;
    }

    for (int i = 0; i < octaves; i ++) {
        total += noise_simplex_4d_permutation(x * scale * f, y * scale * f, z * scale * f, w * scale * f, p) / f;
        t += 1.0 / f;
        f *= 2;
    }

    return total / t;
}