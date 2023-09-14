// Perlin noise algorithm for generating seeded randomized values
// https://rosettacode.org/wiki/Perlin_noise
//
// Example:
//
// int *p = perlin_generate_permutation(SEED);
// float nx = to_float(x) / SIZE;
// float ny = to_float(y) / SIZE;
// float terrain = (1.0 + perlin_noise_2d(nx, ny, p, 8, 15.0)) * 0.5;
//

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

/* ----- noise helper functions ----- */

// Fade eases & smooths coordinate values towards integral values
float noise_fade (float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
// Source: http://riven8192.blogspot.com/2010/08/calculate-perlinnoise-twice-as-fast.html
float noise_grad (int hash, float x, float y, float z) {
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
float noise_lerp (float t, float a, float b) {
    if (0.0 > t) return a;
    if (1.0 < t) return b;
    return a + t * (b - a);
}

/* ----- noise permutation functions ----- */

float noise_2d_permutation (float x, float y, int *p) {
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
    u = noise_fade(x);
    v = noise_fade(y);

    // hash coordinates of the 4 corners
    // https://saturncloud.io/blog/producing-2d-perlin-noise-with-numpy-a-comprehensive-guide-for-data-scientists
    A  = (p[X          ] + Y) & 255;
    A1 = (A + 1             ) & 255;
    B  = (p[(X+1) & 255] + Y) & 255;
    B1 = (B + 1             ) & 255;

    // add blended results from the 4 corners
    return noise_lerp(
        v,
        noise_lerp(
            u,
            noise_grad(p[A ], x  , y  , 0),
            noise_grad(p[B ], x-1, y  , 0)
        ),
        noise_lerp(
            u,
            noise_grad(p[A1], x  , y-1, 0),
            noise_grad(p[B1], x-1, y-1, 0)
        )
    );
}

float noise_3d_permutation (float x, float y, float z, int *p) {
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
    u = noise_fade(x);
    v = noise_fade(y);
    w = noise_fade(z);

    // hash coordinates of the 8 cube corners
    A  = (p[X          ] + Y) & 255;
    AA = (p[A          ] + Z) & 255;
    AB = (p[(A+1) & 255] + Z) & 255;
    B  = (p[(X+1) & 255] + Y) & 255;
    BA = (p[B          ] + Z) & 255;
    BB = (p[(B+1) & 255] + Z) & 255;

    // add blended results from 8 corners of cube
    return noise_lerp(
        w,
        noise_lerp(
            v,
            noise_lerp(
                u,
                noise_grad(p[AA        ], x  , y  , z  ),
                noise_grad(p[BA        ], x-1, y  , z  )
            ),
            noise_lerp(
                u,
                noise_grad(p[AB        ], x  , y-1, z  ),
                noise_grad(p[BB        ], x-1, y-1, z  )
            )
        ),
        noise_lerp(
            v,
            noise_lerp(
                u,
                noise_grad(p[AA+1 & 255], x  , y  , z-1),
                noise_grad(p[BA+1 & 255], x-1, y  , z-1)
            ),
            noise_lerp(
                u,
                noise_grad(p[AB+1 & 255], x  , y-1, z-1),
                noise_grad(p[BB+1 & 255], x-1, y-1, z-1)
            )
        )
    );
}

/* ----- perlin_noise functions ----- */

// generate noise at coordinates x,y using a permutation, octaves, and scale
float perlin_noise_2d (float x, float y, int *p, int octaves, float scale) {
    float total = 0.0;
    float amplitude = 1.0;

    if (!floatp(x)) {
        error("Bad argument 1 to noise->perlin_noise_2d");
    }
    if (!floatp(y)) {
        error("Bad argument 2 to noise->perlin_noise_2d");
    }
    if (!arrayp(p)) {
        error("Bad argument 3 to noise->perlin_noise_2d");
    }
    if (!octaves) {
        octaves = 4;
    }
    if (!scale) {
      scale = 1.0;
    }

    for (int i = 0; i < octaves; i ++) {
        total += noise_2d_permutation(x * scale, y * scale, p) * amplitude;
        scale *= 2.0;
        amplitude *= 0.5;
    }

    return total;
}

// generate noise at coordinates x,y,z using a permutation, octaves, and scale
float perlin_noise_3d (float x, float y, float z, int *p, int octaves, float scale) {
    float total = 0.0;
    float amplitude = 1.0;

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
        total += noise_3d_permutation(x * scale, y * scale, z * scale, p) * amplitude;
        scale *= 2.0;
        amplitude *= 0.5;
    }

    return total;
}

/* ----- perlin seeded permutation ----- */

// uses Xorshift128+ to generate a seeded permutation array
// https://stackoverflow.com/questions/34426499/what-is-the-real-definition-of-the-xorshift128-algorithm
int *perlin_generate_permutation (string seed) {
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