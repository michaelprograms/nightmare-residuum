#include "noise.h"
#include <math.h>
#include <stddef.h>

static const int32_t PERMUTATION[256] = {
151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225,
140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148,
247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117, 35,  11,  32,
57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136, 171, 168, 68,  175,
74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,
65,  25,  63,  161, 1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169,
200, 196, 135, 130, 116, 188, 159, 86,  164, 100, 109, 198, 173, 186, 3,   64,
52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126, 255, 82,  85,  212,
207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213,
119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,
129, 22,  39,  253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104,
218, 246, 97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241,
81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180
};

static const int32_t GRAD4[128] = {
0, 1, 1, 1, 0, 1, 1, -1, 0, 1, -1, 1, 0, 1, -1, -1,
0, -1, 1, 1, 0, -1, 1, -1, 0, -1, -1, 1, 0, -1, -1, -1,
1, 0, 1, 1, 1, 0, 1, -1, 1, 0, -1, 1, 1, 0, -1, -1,
-1, 0, 1, 1, -1, 0, 1, -1, -1, 0, -1, 1, -1, 0, -1, -1,
1, 1, 0, 1, 1, 1, 0, -1, 1, -1, 0, 1, 1, -1, 0, -1,
-1, 1, 0, 1, -1, 1, 0, -1, -1, -1, 0, 1, -1, -1, 0, -1,
1, 1, 1, 0, 1, 1, -1, 0, 1, -1, 1, 0, 1, -1, -1, 0,
-1, 1, 1, 0, -1, 1, -1, 0, -1, -1, 1, 0, -1, -1, -1, 0
};

/* Port of noise_generate_permutation (Xorshift128+ Fisher-Yates) +
   noise_generate_permutation_simplex (gradient resolve). State kept in
   uint64_t so the shift/xor wraparound is well-defined and matches LPC's
   two's-complement 64-bit ints; the modulo step is done through a signed
   cast so it matches LPC's signed % and the j<0 correction. */
void ns_permutation(const uint8_t *seed, int seedlen, ns_perm_t *out) {
    int32_t p[256];
    int i;

    for (i = 0; i < 256; i++) {
        p[i] = PERMUTATION[i];
    }

    if (seed != NULL && seedlen > 0) {
        uint64_t state0 = 0, state1, s0, s1;
        for (i = 0; i < seedlen; i++) {
            state0 += (uint64_t)seed[i];
        }
        state0 &= 0x7fffffffULL;
        state1 = state0 + 0x9E3779B9ULL;
        for (i = 0; i <= 255; i++) {
            int m = 255 - i + 1;
            int64_t sum, j;
            int32_t swap;
            s1 = state0;
            s0 = state1;
            state0 = s0;
            s1 ^= s1 << 23;
            s1 ^= (s1 >> 18) & 0x3FFFFFFFFFFFULL;
            s1 ^= s0;
            s1 ^= (s0 >> 5) & 0x07FFFFFFFFFFFFFFULL;
            state1 = s1;
            sum = (int64_t)(state0 + state1);
            j = sum % m;
            if (j < 0) {
                j += m;
            }
            swap = p[i];
            p[i] = p[i + (int)j];
            p[i + (int)j] = swap;
        }
    }

    for (i = 0; i < 256; i++) {
        int g = (p[i] % 32) * 4;
        out->p[i]  = p[i];
        out->gx[i] = GRAD4[g + 0];
        out->gy[i] = GRAD4[g + 1];
        out->gz[i] = GRAD4[g + 2];
        out->gw[i] = GRAD4[g + 3];
    }
}

/* Port of noise_simplex_4d_permutation (single-octave 4D simplex). */
static double ns_simplex_4d_permutation(double x, double y, double z, double w,
                                        const ns_perm_t *p) {
    double n0, n1, n2, n3, n4;
    double s = (x + y + z + w) * 0.309017;
    int i = (int)floor(x + s);
    int j = (int)floor(y + s);
    int k = (int)floor(z + s);
    int l = (int)floor(w + s);
    double t = (i + j + k + l) * 0.138197;
    double x0 = x - (i - t), y0 = y - (j - t), z0 = z - (k - t), w0 = w - (l - t);
    int rankx = 0, ranky = 0, rankz = 0, rankw = 0;
    int i1, j1, k1, l1, i2, j2, k2, l2, i3, j3, k3, l3;
    double x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4;
    int ii, jj, kk, ll;
    double t0, t1, t2, t3, t4;
    int gi0, gi1, gi2, gi3, gi4;

    if (x0 > y0) rankx++; else ranky++;
    if (x0 > z0) rankx++; else rankz++;
    if (x0 > w0) rankx++; else rankw++;
    if (y0 > z0) ranky++; else rankz++;
    if (y0 > w0) ranky++; else rankw++;
    if (z0 > w0) rankz++; else rankw++;

    i1 = rankx >= 3 ? 1 : 0; j1 = ranky >= 3 ? 1 : 0;
    k1 = rankz >= 3 ? 1 : 0; l1 = rankw >= 3 ? 1 : 0;
    i2 = rankx >= 2 ? 1 : 0; j2 = ranky >= 2 ? 1 : 0;
    k2 = rankz >= 2 ? 1 : 0; l2 = rankw >= 2 ? 1 : 0;
    i3 = rankx >= 1 ? 1 : 0; j3 = ranky >= 1 ? 1 : 0;
    k3 = rankz >= 1 ? 1 : 0; l3 = rankw >= 1 ? 1 : 0;

    x1 = x0 - i1 + 0.138197; y1 = y0 - j1 + 0.138197;
    z1 = z0 - k1 + 0.138197; w1 = w0 - l1 + 0.138197;
    x2 = x0 - i2 + 2.0 * 0.138197; y2 = y0 - j2 + 2.0 * 0.138197;
    z2 = z0 - k2 + 2.0 * 0.138197; w2 = w0 - l2 + 2.0 * 0.138197;
    x3 = x0 - i3 + 3.0 * 0.138197; y3 = y0 - j3 + 3.0 * 0.138197;
    z3 = z0 - k3 + 3.0 * 0.138197; w3 = w0 - l3 + 3.0 * 0.138197;
    x4 = x0 - 1.0 + 4.0 * 0.138197; y4 = y0 - 1.0 + 4.0 * 0.138197;
    z4 = z0 - 1.0 + 4.0 * 0.138197; w4 = w0 - 1.0 + 4.0 * 0.138197;

    ii = i & 255; jj = j & 255; kk = k & 255; ll = l & 255;

    t0 = 0.6 - x0*x0 - y0*y0 - z0*z0 - w0*w0;
    if (t0 < 0) {
        n0 = 0.0;
    } else {
        gi0 = ii + p->p[(jj + p->p[(kk + p->p[ll]) & 255]) & 255];
        t0 *= t0;
        n0 = t0*t0*(p->gx[gi0&255]*x0 + p->gy[gi0&255]*y0 + p->gz[gi0&255]*z0 + p->gw[gi0&255]*w0);
    }
    t1 = 0.6 - x1*x1 - y1*y1 - z1*z1 - w1*w1;
    if (t1 < 0) {
        n1 = 0.0;
    } else {
        gi1 = ii + i1 + p->p[(jj + j1 + p->p[(kk + k1 + p->p[(ll + l1)&255])&255])&255];
        t1 *= t1;
        n1 = t1*t1*(p->gx[gi1&255]*x1 + p->gy[gi1&255]*y1 + p->gz[gi1&255]*z1 + p->gw[gi1&255]*w1);
    }
    t2 = 0.6 - x2*x2 - y2*y2 - z2*z2 - w2*w2;
    if (t2 < 0) {
        n2 = 0.0;
    } else {
        gi2 = ii + i2 + p->p[(jj + j2 + p->p[(kk + k2 + p->p[(ll + l2)&255])&255])&255];
        t2 *= t2;
        n2 = t2*t2*(p->gx[gi2&255]*x2 + p->gy[gi2&255]*y2 + p->gz[gi2&255]*z2 + p->gw[gi2&255]*w2);
    }
    t3 = 0.6 - x3*x3 - y3*y3 - z3*z3 - w3*w3;
    if (t3 < 0) {
        n3 = 0.0;
    } else {
        gi3 = ii + i3 + p->p[(jj + j3 + p->p[(kk + k3 + p->p[(ll + l3)&255])&255])&255];
        t3 *= t3;
        n3 = t3*t3*(p->gx[gi3&255]*x3 + p->gy[gi3&255]*y3 + p->gz[gi3&255]*z3 + p->gw[gi3&255]*w3);
    }
    t4 = 0.6 - x4*x4 - y4*y4 - z4*z4 - w4*w4;
    if (t4 < 0) {
        n4 = 0.0;
    } else {
        gi4 = ii + 1 + p->p[(jj + 1 + p->p[(kk + 1 + p->p[(ll + 1)&255])&255])&255];
        t4 *= t4;
        n4 = t4*t4*(p->gx[gi4&255]*x4 + p->gy[gi4&255]*y4 + p->gz[gi4&255]*z4 + p->gw[gi4&255]*w4);
    }
    return 27.0 * (n0 + n1 + n2 + n3 + n4);
}

double ns_simplex_4d(double x, double y, double z, double w,
                     const ns_perm_t *perm, int octaves, double scale) {
    double total = 0.0, t = 0.0;
    int f = 1, i;
    if (octaves == 0) {
        octaves = 4;
    }
    if (scale == 0.0) {
        scale = 1.0;
    }
    for (i = 0; i < octaves; i++) {
        total += ns_simplex_4d_permutation(x*scale*f, y*scale*f, z*scale*f, w*scale*f, perm) / f;
        t += 1.0 / f;
        f *= 2;
    }
    return total / t;
}

void ns_simplex_4d_row(double *out, int count,
                       double x0, double dx, double y, double z, double w,
                       const ns_perm_t *perm, int octaves, double scale) {
    int k;
    for (k = 0; k < count; k++) {
        out[k] = ns_simplex_4d(x0 + k*dx, y, z, w, perm, octaves, scale);
    }
}

#ifdef NS_SELFTEST
#include <stdio.h>
int main(void) {
    ns_perm_t perm;
    const char *seed = "Terra";
    ns_permutation((const uint8_t *)seed, 5, &perm);
    printf("Terra (0.1,0.2,0.3,0.4) oct5 scale1.25 = %.15f\n",
           ns_simplex_4d(0.1, 0.2, 0.3, 0.4, &perm, 5, 1.25));
    printf("Terra (1.5,2.5,3.5,4.5) oct5 scale1.25 = %.15f\n",
           ns_simplex_4d(1.5, 2.5, 3.5, 4.5, &perm, 5, 1.25));
    return 0;
}
#endif
