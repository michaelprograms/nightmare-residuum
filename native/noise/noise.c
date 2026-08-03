#include "noise.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

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

/* ----- planet cell/row (port of D_PLANET query_noise, lib/daemon/planet.c) -----
   Batches the whole per-cell biome-shaping computation so a full planet costs
   ~size FFI calls instead of ~size*size*6. Values must stay bit-parity with the
   LPC query_noise. Output packs 5 doubles per cell:
   [0]=level [1]=height [2]=humidity [3]=heat [4]=resource. */

#define NS_PIx2          6.283185307179586
#define NS_LEVEL_RANGE   20
#define NS_WATER_LAKES   0.75
#define NS_WATER_RIVER_1 0.60
#define NS_WATER_RIVER_2 0.40
#define NS_HEIGHT_DEEPER 0.35
#define NS_HEIGHT_DEEP   0.40
#define NS_HEIGHT_SHALLOW 0.45
#define NS_HEIGHT_SHORE  0.50

static double ns_gradient_2d(double x1, double y1, double x2, double y2,
                             double x, double y) {
    double dX = x2 - x1, dY = y2 - y1;
    double denominator = dX * dX + dY * dY;
    if (denominator == 0.0) {
        return 0.0;
    }
    return ((x - x1) * dX + (y - y1) * dY) / denominator;
}

void ns_planet_cell(const ns_perm_t *perm, int size, int x, int y,
                    double heightFactor, double humidityFactor,
                    double heatFactor, double now, double *out5) {
    int size2 = size / 2, hs2, level;
    double nx, ny, nz, nw, nowAdj = now;
    double nHeight, nHumidity = 0.0, nHeat = 0.0, nTmp, nResource = -1.0;
    double dl;

    nx = cos(((double)x / size) * NS_PIx2) * 2 / NS_PIx2;
    ny = cos(((double)y / size) * NS_PIx2) * 2 / NS_PIx2;
    nz = sin(((double)x / size) * NS_PIx2) * 2 / NS_PIx2;
    nw = sin(((double)y / size) * NS_PIx2) * 2 / NS_PIx2;
    if (nowAdj > 0.5) {
        nowAdj -= 0.5;
    }

    /* level: radial distance from center, clamped to [1, LEVEL_RANGE] */
    dl = sqrt((double)(size2 - x) * (size2 - x) +
              (double)(size2 - y) * (size2 - y));
    level = (int)(dl / (size2 * 1.0 / NS_LEVEL_RANGE));
    if (level < 1) {
        level = 1;
    }
    if (level > NS_LEVEL_RANGE) {
        level = NS_LEVEL_RANGE;
    }

    if (heatFactor != 0.0) {
        nHumidity = (((ns_simplex_4d(nx + nowAdj, ny + nowAdj, nz + nowAdj,
                      nw + nowAdj, perm, 4, 3.0) + 1) / 2) - 0.25) / 0.5;
        if (nHumidity < 0.0) {
            nHumidity = 0.0;
        }
        nHumidity *= humidityFactor;
    }

    nHeight = (((ns_simplex_4d(nx, ny, nz, nw, perm, 5, 1.25) + 1) / 2) - 0.25)
              / 0.5;
    nHeight *= heightFactor;
    if ((nHeight <= NS_HEIGHT_SHALLOW) &&
        (x <= size2 + 3 && y <= size2 + 3 && x >= size2 - 3 &&
         y >= size2 - 3) &&
        (sqrt((double)(size2 - x) * (size2 - x) +
              (double)(size2 - y) * (size2 - y)) + 0.5 < 3)) {
        nHeight += (NS_HEIGHT_SHALLOW - nHeight) + 0.05;
    } else {
        nTmp = (((ns_simplex_4d(nx, ny, nz, nw, perm, 4, 1.25) + 1) / 2) - 0.25)
               / 0.5;
        if (nTmp >= NS_WATER_LAKES - 0.026 && nTmp <= NS_WATER_LAKES + 0.026) {
            if (nTmp >= NS_WATER_LAKES - 0.025 &&
                nTmp <= NS_WATER_LAKES + 0.025) {
                double c = nHeight - (fabs(nHeight - NS_HEIGHT_SHALLOW) + 0.05);
                nHeight = c > 0.0 ? c : 0.0;
            }
            if (heatFactor != 0.0) {
                nHumidity += 0.1 * nHeight;
            }
        } else {
            nTmp = (((ns_simplex_4d(nw, nz, ny, nx, perm, 5, 1.25) + 1) / 2) -
                   0.25) / 0.5;
            if ((nTmp >= NS_WATER_RIVER_1 - 0.026 &&
                 nTmp <= NS_WATER_RIVER_1 + 0.026) ||
                (nTmp >= NS_WATER_RIVER_2 - 0.026 &&
                 nTmp <= NS_WATER_RIVER_1 + 0.026)) {
                if ((nTmp >= NS_WATER_RIVER_1 - 0.025 &&
                     nTmp <= NS_WATER_RIVER_1 + 0.025) ||
                    (nTmp >= NS_WATER_RIVER_2 - 0.025 &&
                     nTmp <= NS_WATER_RIVER_2 + 0.025)) {
                    if (nHeight <= NS_HEIGHT_SHALLOW) {
                        nHeight = nHeight * 0.8;
                    } else {
                        double c = nHeight -
                                   (fabs(nHeight - NS_HEIGHT_SHALLOW) + 0.05);
                        nHeight = c > 0.0 ? c : 0.0;
                    }
                }
                if (heatFactor != 0.0) {
                    nHumidity += 0.1 * nHeight;
                }
            }
        }
    }

    if (heatFactor != 0.0) {
        if (nHeight <= NS_HEIGHT_DEEPER) {
            nHumidity += 5.0 * nHeight;
        } else if (nHeight <= NS_HEIGHT_DEEP) {
            nHumidity += 4.0 * nHeight;
        } else if (nHeight <= NS_HEIGHT_SHALLOW) {
            nHumidity += 3.0 * nHeight;
        } else if (nHeight <= NS_HEIGHT_SHORE) {
            nHumidity += 0.2 * nHeight;
        }

        nHeat = fabs((ns_simplex_4d(nx + nowAdj, ny + nowAdj, nz + nowAdj,
                     nw + nowAdj, perm, 4, 2.5) + 1) / 2);
        nHeat = fabs((nHeat - 0.05) / (0.95 - 0.05));
        hs2 = size * 45 / 100;
        if (y >= hs2 && y <= size - hs2) {
            nTmp = 1.0;
        } else if (y < hs2) {
            nTmp = 1.0 - ns_gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0,
                                        (y * 1.0) / hs2);
        } else {
            nTmp = 1.0 - ns_gradient_2d(1.0, 1.0, 1.0, 0.0, 0.0,
                                        (size - (y + 1.0)) / hs2);
        }
        nHeat = nHeat * nTmp * 1.5;
        nHeat *= heatFactor;
        if (nHeat > 1.0) {
            nHeat = 1.0;
        }
        if (nHeight > NS_HEIGHT_SHALLOW) {
            nHeat -= (nHeight - NS_HEIGHT_SHALLOW) * nHeight;
        }
        if (nHeat < 0.0) {
            nHeat = 0.0;
        }
    }

    if (nHeight > NS_HEIGHT_SHORE) {
        nResource = ((ns_simplex_4d(nx + now, ny + now, nz + now, nw + now,
                     perm, 4, 25.0) + 1) / 2);
    }

    out5[0] = (double)level;
    out5[1] = nHeight;
    out5[2] = nHumidity;
    out5[3] = nHeat;
    out5[4] = (double)((int)(nResource * 100.0) % 10);
}

void ns_planet_row(const ns_perm_t *perm, int size, int y,
                   double heightFactor, double humidityFactor,
                   double heatFactor, double now, double *out) {
    int x;
    for (x = 0; x < size; x++) {
        ns_planet_cell(perm, size, x, y, heightFactor, humidityFactor,
                       heatFactor, now, &out[x * 5]);
    }
}

/* ----- planet PNG (port of planet.c color tables + query_biome, laid out to
   match tools/planet-render/print-planet.js). Colors are packed 0xRRGGBB.
   The panel blends replicate shade-blend-color pSBC: channel =
   round((to-from)*|p| + from), |p| clamped to [0,1]. ----- */

#define NS_HEAT_COLDEST  0.05
#define NS_HEAT_COLDER   0.15
#define NS_HEAT_COLD     0.35
#define NS_HEAT_HOT      0.50
#define NS_HEAT_HOTTER   0.70
#define NS_HUM_DRYEST    0.20
#define NS_HUM_DRYER     0.45
#define NS_HUM_DRY       0.55
#define NS_HUM_WET       0.65
#define NS_HUM_WETTER    1.00
#define NS_HUM_WETTEST   10.0
#define NS_STRUCTURE_RARITY 4

/* query_biome + __BiomeColor: (height, heat, humidity) -> biome RGB. */
static uint32_t ns_biome_color(double height, double heat, double humidity) {
    if (height <= NS_HEIGHT_DEEPER) {
        if (heat <= NS_HEAT_COLDEST) {
            return 0xBDDBF6;  /* frozen water */
        } else if (heat <= NS_HEAT_COLDER) {
            return 0x69BDE6;  /* icy water */
        }
        return 0x000060;  /* deeper water */
    } else if (height <= NS_HEIGHT_DEEP) {
        if (heat <= NS_HEAT_COLDEST) {
            return 0xBDDBF6;
        } else if (heat <= NS_HEAT_COLDER) {
            return 0x69BDE6;
        }
        return 0x000080;  /* deep water */
    } else if (height <= NS_HEIGHT_SHALLOW) {
        if (heat <= NS_HEAT_COLDEST) {
            return 0xBDDBF6;
        } else if (heat <= NS_HEAT_COLDER) {
            return 0x69BDE6;
        }
        return 0x191996;  /* shallow water */
    } else if (heat <= NS_HEAT_COLDEST) {
        return 0xFFFFFF;  /* ice */
    } else if (heat <= NS_HEAT_COLDER) {
        return 0x608370;  /* tundra */
    } else if (heat <= NS_HEAT_COLD) {
        if (humidity <= NS_HUM_DRYER) {
            return 0xA4FF63;  /* grassland */
        } else if (humidity <= NS_HUM_DRY) {
            return 0x8BAF5A;  /* woodland */
        }
        return 0x5F733E;  /* boreal forest */
    } else if (heat <= NS_HEAT_HOT) {
        if (humidity <= NS_HUM_DRYER) {
            return 0xEEDA82;  /* desert */
        } else if (humidity <= NS_HUM_DRY) {
            return 0x8BAF5A;  /* woodland */
        }
        return 0x1D4928;  /* temperate rainforest */
    } else if (heat <= NS_HEAT_HOTTER) {
        if (humidity <= NS_HUM_DRYER) {
            return 0xEEDA82;  /* desert */
        } else if (humidity <= NS_HUM_WET) {
            return 0xB1D16E;  /* savanna */
        }
        return 0x427B19;  /* tropical rainforest */
    }
    if (humidity <= NS_HUM_DRYER) {
        return 0xEEDA82;  /* desert */
    } else if (humidity <= NS_HUM_WET) {
        return 0xB1D16E;  /* savanna */
    }
    return 0x427B19;  /* tropical rainforest */
}

/* query_humidity_color_hex: ascending thresholds. */
static uint32_t ns_humidity_color(double humidity) {
    if (humidity <= NS_HUM_DRYEST) {
        return 0xFF8B11;
    } else if (humidity <= NS_HUM_DRYER) {
        return 0x50FF00;
    } else if (humidity <= NS_HUM_DRY) {
        return 0x50FF00;
    } else if (humidity <= NS_HUM_WET) {
        return 0x55FFFF;
    } else if (humidity <= NS_HUM_WETTER) {
        return 0x1446FF;
    } else if (humidity <= NS_HUM_WETTEST) {
        return 0x000064;
    }
    return 0x000000;
}

/* query_heat_color_hex: ascending thresholds. */
static uint32_t ns_heat_color(double heat) {
    if (heat <= NS_HEAT_COLDEST) {
        return 0x00FFFF;
    } else if (heat <= NS_HEAT_COLDER) {
        return 0xAAFFFF;
    } else if (heat <= NS_HEAT_COLD) {
        return 0x00E585;
    } else if (heat <= NS_HEAT_HOT) {
        return 0xFFFF64;
    } else if (heat <= NS_HEAT_HOTTER) {
        return 0xFF6400;
    } else if (heat <= 1.0) {
        return 0xF10C00;
    }
    return 0x000000;
}

/* query_resource_color_hex: 0 -> black, 1 -> silver, 2 -> copper, else black. */
static uint32_t ns_resource_color(int resource) {
    if (resource == 1) {
        return 0xC0C0C0;
    } else if (resource == 2) {
        return 0xA47449;
    }
    return 0x000000;
}

static int ns_round_u8(double v) {
    int r = (int)(v + 0.5);
    if (r < 0) {
        r = 0;
    }
    if (r > 255) {
        r = 255;
    }
    return r;
}

/* height panel: pSBC(|floor(h*20)/20|, #000000, #FFFFFF) -> gray. */
static uint32_t ns_height_gray(double height) {
    double p = fabs(floor(height * 20.0) / 20.0);
    int v;
    if (p > 1.0) {
        p = 1.0;
    }
    v = ns_round_u8(255.0 * p);
    return ((uint32_t)v << 16) | ((uint32_t)v << 8) | (uint32_t)v;
}

/* level panel: pSBC(level/20, #FFFFFF, #7F0000). */
static uint32_t ns_level_color(int level) {
    double p = level / (double)NS_LEVEL_RANGE;
    int r, g, b;
    if (p > 1.0) {
        p = 1.0;
    }
    r = ns_round_u8((127 - 255) * p + 255);
    g = ns_round_u8((0 - 255) * p + 255);
    b = ns_round_u8((0 - 255) * p + 255);
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

static void ns_set_px(uint8_t *rgb, int W, int px, int py, uint32_t color) {
    size_t off = ((size_t)py * W + px) * 3;
    rgb[off + 0] = (color >> 16) & 255;
    rgb[off + 1] = (color >> 8) & 255;
    rgb[off + 2] = color & 255;
}

static void ns_png_chunk(FILE *f, const char *type, const uint8_t *data,
                         size_t len) {
    uint8_t hdr[4];
    uint8_t crcb[4];
    uLong crc;
    hdr[0] = (len >> 24) & 255;
    hdr[1] = (len >> 16) & 255;
    hdr[2] = (len >> 8) & 255;
    hdr[3] = len & 255;
    fwrite(hdr, 1, 4, f);
    fwrite(type, 1, 4, f);
    if (data && len) {
        fwrite(data, 1, len, f);
    }
    crc = crc32(0L, (const Bytef *)type, 4);
    if (data && len) {
        crc = crc32(crc, data, len);
    }
    crcb[0] = (crc >> 24) & 255;
    crcb[1] = (crc >> 16) & 255;
    crcb[2] = (crc >> 8) & 255;
    crcb[3] = crc & 255;
    fwrite(crcb, 1, 4, f);
}

static int ns_write_png(const char *path, int W, int H, const uint8_t *rgb) {
    static const uint8_t sig[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
    size_t stride = (size_t)W * 3;
    size_t rawlen = (size_t)H * (1 + stride);
    uint8_t ihdr[13];
    uint8_t *raw, *comp;
    uLongf clen;
    FILE *f;
    int y;

    raw = malloc(rawlen);
    if (!raw) {
        return 0;
    }
    for (y = 0; y < H; y++) {
        raw[(size_t)y * (1 + stride)] = 0;  /* filter type 0 (none) */
        memcpy(raw + (size_t)y * (1 + stride) + 1, rgb + (size_t)y * stride,
               stride);
    }
    clen = compressBound(rawlen);
    comp = malloc(clen);
    if (!comp) {
        free(raw);
        return 0;
    }
    if (compress2(comp, &clen, raw, rawlen, Z_BEST_SPEED) != Z_OK) {
        free(raw);
        free(comp);
        return 0;
    }
    free(raw);

    f = fopen(path, "wb");
    if (!f) {
        free(comp);
        return 0;
    }
    fwrite(sig, 1, 8, f);
    ihdr[0] = (W >> 24) & 255;
    ihdr[1] = (W >> 16) & 255;
    ihdr[2] = (W >> 8) & 255;
    ihdr[3] = W & 255;
    ihdr[4] = (H >> 24) & 255;
    ihdr[5] = (H >> 16) & 255;
    ihdr[6] = (H >> 8) & 255;
    ihdr[7] = H & 255;
    ihdr[8] = 8;   /* bit depth */
    ihdr[9] = 2;   /* color type 2 = truecolor RGB */
    ihdr[10] = 0;
    ihdr[11] = 0;
    ihdr[12] = 0;
    ns_png_chunk(f, "IHDR", ihdr, 13);
    ns_png_chunk(f, "IDAT", comp, clen);
    ns_png_chunk(f, "IEND", NULL, 0);
    fclose(f);
    free(comp);
    return 1;
}

/* Render a full planet to a PNG in one call. Layout (W = size*2, H = size*4):
   (0,0) biome        (size,0) height
   (0,size) humidity  (size,size) heat
   (0,2s) level       (size,2s) resource
   (0,3s) structures on black   (size,3s) biome + structure markers
   Returns 1 on success, 0 on failure. */
int ns_planet_png(const ns_perm_t *perm, int size, double heightFactor,
                  double humidityFactor, double heatFactor, double now,
                  const char *path, int pathlen) {
    int W = size * 2, H = size * 4;
    uint8_t *rgb;
    char safe[512];
    int x, y, ok;

    if (pathlen <= 0 || pathlen >= (int)sizeof(safe)) {
        return 0;
    }
    memcpy(safe, path, pathlen);
    safe[pathlen] = 0;

    rgb = calloc((size_t)W * H, 3);  /* zeroed: row-4 left starts black */
    if (!rgb) {
        return 0;
    }
    for (y = 0; y < size; y++) {
        for (x = 0; x < size; x++) {
            double out5[5];
            double height, humidity, heat;
            int level, resource;
            uint32_t biome;
            int gate;
            ns_planet_cell(perm, size, x, y, heightFactor, humidityFactor,
                           heatFactor, now, out5);
            level = (int)(out5[0] + 0.5);
            height = out5[1];
            humidity = out5[2];
            heat = out5[3];
            resource = (int)(out5[4] + 0.5);
            biome = ns_biome_color(height, heat, humidity);

            ns_set_px(rgb, W, x, y, biome);
            ns_set_px(rgb, W, x + size, y, ns_height_gray(height));
            ns_set_px(rgb, W, x, y + size, ns_humidity_color(humidity));
            ns_set_px(rgb, W, x + size, y + size, ns_heat_color(heat));
            ns_set_px(rgb, W, x, y + size * 2, ns_level_color(level));
            ns_set_px(rgb, W, x + size, y + size * 2,
                      ns_resource_color(resource));
            ns_set_px(rgb, W, x + size, y + size * 3, biome);

            /* structure gate mirrors query_structure/scan_structures */
            gate = perm->p[(perm->p[x & 255] + (y & 255)) & 255];
            if (gate < NS_STRUCTURE_RARITY && height > NS_HEIGHT_SHORE) {
                int styp = perm->p[(perm->p[y & 255] + (x & 255)) & 255] % 2;
                uint32_t sc = styp == 0 ? 0xFF00FF : 0xFF0000;
                ns_set_px(rgb, W, x, y + size * 3, sc);
                ns_set_px(rgb, W, x + size, y + size * 3, sc);
            }
        }
    }
    ok = ns_write_png(safe, W, H, rgb);
    free(rgb);
    return ok;
}

#ifdef NS_PNGGEN
int main(int argc, char **argv) {
    ns_perm_t perm;
    const char *seed = argc > 1 ? argv[1] : "Terra";
    int size = argc > 2 ? atoi(argv[2]) : 500;
    double now = argc > 3 ? atof(argv[3]) : 0.5;
    char path[256];
    int n;
    ns_permutation((const uint8_t *)seed, strlen(seed), &perm);
    n = snprintf(path, sizeof(path), "%s-%d.png", seed, size);
    if (ns_planet_png(&perm, size, 1.0, 1.0, 1.0, now, path, n)) {
        printf("wrote %s (%dx%d)\n", path, size * 2, size * 4);
        return 0;
    }
    printf("FAILED\n");
    return 1;
}
#endif

#ifdef NS_BENCH
#include <time.h>
int main(int argc, char **argv) {
    ns_perm_t perm;
    const char *seed = "Earth";
    int size = argc > 1 ? atoi(argv[1]) : 500;
    int y;
    double *row = malloc(sizeof(double) * size * 5);
    double checksum = 0.0;
    struct timespec t0, t1;
    double ms;

    ns_permutation((const uint8_t *)seed, 5, &perm);
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (y = 0; y < size; y++) {
        int i;
        ns_planet_row(&perm, size, y, 1.0, 1.0, 1.0, 0.5, row);
        for (i = 0; i < size * 5; i++) {
            checksum += row[i];
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t1);
    ms = (t1.tv_sec - t0.tv_sec) * 1000.0 +
         (t1.tv_nsec - t0.tv_nsec) / 1000000.0;
    printf("planet %s size %d: %.2f ms (checksum %.6f)\n", seed, size, ms,
           checksum);
    free(row);
    return 0;
}
#endif

#ifdef NS_SELFTEST
/* Standalone parity harness: every expected value below was produced by the
   LPC reference (lib/secure/sefun/noise.test.c and lib/daemon/planet.test.c),
   so a passing run proves this C port matches LPC without needing the driver.
   Build+run via native/noise/build.sh. */
#include <stdio.h>

static int ns_fail = 0;

static void check_int(const char *label, long got, long expected) {
    if (got != expected) {
        printf("FAIL %-32s got %ld expected %ld\n", label, got, expected);
        ns_fail = 1;
    }
}

static void check_dbl(const char *label, double got, double expected) {
    double d = got - expected;
    if (d < 0) {
        d = -d;
    }
    if (d > 1e-5) {
        printf("FAIL %-32s got %.6f expected %.6f (|d|=%.2e)\n", label, got,
               expected, d);
        ns_fail = 1;
    }
}

/* noise_generate_permutation("test"), from test_generate_permutation. */
static const int32_t PERM_TEST[256] = {
31, 240, 218, 134, 161, 209, 51, 19, 178, 130, 75, 154, 245, 123, 107, 117,
69, 129, 163, 156, 247, 125, 16, 235, 166, 244, 121, 180, 242, 202, 172, 203,
68, 67, 207, 56, 197, 233, 4, 193, 200, 23, 12, 187, 88, 101, 30, 179,
108, 147, 143, 225, 116, 226, 21, 122, 192, 47, 50, 157, 201, 251, 65, 13,
81, 52, 213, 74, 224, 10, 133, 199, 49, 182, 44, 103, 114, 171, 96, 45,
109, 83, 205, 255, 250, 6, 54, 9, 228, 254, 120, 36, 124, 145, 175, 153,
188, 77, 249, 43, 76, 33, 127, 139, 151, 212, 92, 186, 196, 110, 131, 113,
189, 20, 84, 90, 152, 160, 105, 38, 185, 239, 7, 98, 162, 219, 204, 82,
216, 86, 149, 61, 170, 214, 252, 155, 39, 78, 148, 118, 194, 230, 237, 73,
41, 183, 27, 1, 234, 128, 66, 14, 164, 112, 223, 248, 62, 72, 29, 229,
174, 100, 144, 119, 169, 231, 190, 5, 181, 48, 158, 79, 89, 126, 167, 46,
221, 106, 217, 58, 253, 53, 137, 59, 37, 97, 227, 206, 159, 22, 15, 208,
25, 184, 165, 135, 42, 241, 35, 55, 115, 34, 11, 132, 64, 191, 93, 220,
28, 70, 236, 40, 243, 80, 140, 3, 17, 85, 146, 232, 104, 60, 142, 87,
0, 8, 210, 94, 198, 173, 222, 211, 24, 246, 215, 138, 141, 32, 95, 57,
26, 71, 168, 2, 136, 18, 150, 195, 238, 176, 111, 91, 99, 177, 102, 63
};

/* query_noise golden: seed "test", size 100, time 1717171717 (now = 0.74),
   from lib/daemon/planet.test.c test_noise. */
struct planet_golden {
    int x, y, level, resource;
    double height, humidity, heat, hf, humf, hef;
    const char *note;
};
static const struct planet_golden PLANET_GOLDEN[] = {
    {49, 49, 1, 0, 0.500000, 0.126456, 0.034061, 0.1, 0.1, 0.1, "central land"},
    {49, 49, 1, 8, 0.710818, 0.335642, 0.405221, 1.0, 1.0, 1.0, "land"},
    {49, 50, 1, 0, 0.400000, 1.923840, 0.642297, 1.0, 1.0, 1.0, "land"},
    {50, 49, 1, 1, 0.677214, 0.414065, 0.487084, 1.0, 1.0, 1.0, "land"},
    {50, 50, 1, 9, 0.683646, 0.387248, 0.556462, 1.0, 1.0, 1.0, "land"},
    {70, 0, 20, 6, 0.586605, 0.229419, 0.000000, 1.0, 1.0, 1.0, "lake"},
    {8, 0, 20, 4, 0.622579, 0.380266, 0.000000, 1.0, 1.0, 1.0, "water"},
    {14, 0, 20, 4, 0.610453, 0.236256, 0.000000, 1.0, 1.0, 1.0, "water"},
    {46, 0, 20, 0, 0.351678, 1.784100, 0.000000, 1.0, 1.0, 1.0, "humid deeper"},
    {45, 0, 20, 0, 0.344870, 2.114431, 0.000000, 1.0, 1.0, 1.0, "humid deep"},
    {44, 0, 20, 0, 0.419358, 1.542528, 0.000000, 1.0, 1.0, 1.0, "humid shallow"},
    {38, 0, 20, 0, 0.382510, 2.246161, 0.000000, 1.0, 1.0, 1.0, "humid shore"},
    {0, 99, 20, 1, 0.621600, 0.442500, 0.000000, 1.0, 1.0, 1.0, "heat north"},
    {95, 50, 18, 0, 0.400000, 2.444506, 0.773610, 1.0, 1.0, 1.0, "heat center"},
    {99, 99, 20, 2, 0.632431, 0.438998, 0.000000, 1.0, 1.0, 1.0, "heat south"},
    {49, 45, 2, 4, 0.623245, 0.237010, 0.306450, 1.0, 1.0, 1.0, "heat cap"},
};

int main(void) {
    ns_perm_t perm0, permT;
    int i;
    char lbl[48];

    /* --- permutation parity: seeded Xorshift128+ / Fisher-Yates port --- */
    ns_permutation((const uint8_t *)"test", 4, &permT);
    for (i = 0; i < 256; i++) {
        sprintf(lbl, "perm[test][%d]", i);
        check_int(lbl, permT.p[i], PERM_TEST[i]);
    }
    ns_permutation(NULL, 0, &perm0);  /* unseeded == base table */
    check_int("perm[default][0]", perm0.p[0], 151);
    check_int("perm[default][255]", perm0.p[255], 180);

    /* --- simplex parity (single octave == octaves 1, scale 1.0) --- */
    check_dbl("simplex0 (3.14,42,7,0)",
              ns_simplex_4d(3.14, 42.0, 7.0, 0.0, &perm0, 1, 1.0), 0.653330);
    check_dbl("simplex0 (0.5,0.5,0.5,0)",
              ns_simplex_4d(0.5, 0.5, 0.5, 0.0, &perm0, 1, 1.0), -0.463971);
    check_dbl("simplex0 (0.1,0.9,0.5,0)",
              ns_simplex_4d(0.1, 0.9, 0.5, 0.0, &perm0, 1, 1.0), 0.152880);
    check_dbl("simplex0 (0.01,0.01,0.01,0)",
              ns_simplex_4d(0.01, 0.01, 0.01, 0.0, &perm0, 1, 1.0), 0.069844);
    check_dbl("simplexT (3.14,42,7,0)",
              ns_simplex_4d(3.14, 42.0, 7.0, 0.0, &permT, 1, 1.0), -0.480013);
    check_dbl("simplexT (0.5,0.5,0.5,0)",
              ns_simplex_4d(0.5, 0.5, 0.5, 0.0, &permT, 1, 1.0), 0.154212);
    check_dbl("simplexT (0.1,0.9,0.5,0)",
              ns_simplex_4d(0.1, 0.9, 0.5, 0.0, &permT, 1, 1.0), 0.072479);
    check_dbl("simplexT (0.01,0.01,0.01,0)",
              ns_simplex_4d(0.01, 0.01, 0.01, 0.0, &permT, 1, 1.0), 0.000000);

    /* --- fractal simplex (multi-octave) --- */
    check_dbl("simplex0 oct8 s15",
              ns_simplex_4d(3.14, 42.0, 7.0, 0.0, &perm0, 8, 15.0), 0.108415);
    check_dbl("simplexT oct8 s15",
              ns_simplex_4d(3.14, 42.0, 7.0, 0.0, &permT, 8, 15.0), -0.405191);

    /* --- octave/scale defaults: 0 octaves -> 4, 0.0 scale -> 1.0 --- */
    check_dbl("simplex defaults",
              ns_simplex_4d(0.0, 0.0, 0.0, 0.0, &permT, 0, 0.0),
              ns_simplex_4d(0.0, 0.0, 0.0, 0.0, &permT, 4, 1.0));

    /* --- planet cell parity vs query_noise --- */
    for (i = 0; i < (int)(sizeof(PLANET_GOLDEN) / sizeof(PLANET_GOLDEN[0]));
         i++) {
        const struct planet_golden *g = &PLANET_GOLDEN[i];
        double out5[5];
        ns_planet_cell(&permT, 100, g->x, g->y, g->hf, g->humf, g->hef, 0.74,
                       out5);
        sprintf(lbl, "cell(%d,%d) level [%s]", g->x, g->y, g->note);
        check_int(lbl, (long)(out5[0] + 0.5), g->level);
        sprintf(lbl, "cell(%d,%d) height [%s]", g->x, g->y, g->note);
        check_dbl(lbl, out5[1], g->height);
        sprintf(lbl, "cell(%d,%d) humidity [%s]", g->x, g->y, g->note);
        check_dbl(lbl, out5[2], g->humidity);
        sprintf(lbl, "cell(%d,%d) heat [%s]", g->x, g->y, g->note);
        check_dbl(lbl, out5[3], g->heat);
        sprintf(lbl, "cell(%d,%d) resource [%s]", g->x, g->y, g->note);
        check_int(lbl, (long)(out5[4] + 0.5), g->resource);
    }

    /* --- color LUT parity vs planet.c color tables (RGB as 0xRRGGBB) --- */
    check_int("biome deeper", ns_biome_color(0.30, 0.5, 0.5), 0x000060);
    check_int("biome frozen", ns_biome_color(0.30, 0.03, 0.5), 0xBDDBF6);
    check_int("biome icy", ns_biome_color(0.30, 0.10, 0.5), 0x69BDE6);
    check_int("biome deep", ns_biome_color(0.38, 0.5, 0.5), 0x000080);
    check_int("biome shallow", ns_biome_color(0.43, 0.5, 0.5), 0x191996);
    check_int("biome ice", ns_biome_color(0.60, 0.03, 0.5), 0xFFFFFF);
    check_int("biome tundra", ns_biome_color(0.60, 0.10, 0.5), 0x608370);
    check_int("biome grassland", ns_biome_color(0.60, 0.30, 0.40), 0xA4FF63);
    check_int("biome woodland", ns_biome_color(0.60, 0.30, 0.50), 0x8BAF5A);
    check_int("biome boreal", ns_biome_color(0.60, 0.30, 0.70), 0x5F733E);
    check_int("biome desert", ns_biome_color(0.60, 0.50, 0.40), 0xEEDA82);
    check_int("biome temperate", ns_biome_color(0.60, 0.50, 0.70), 0x1D4928);
    check_int("biome savanna", ns_biome_color(0.60, 0.60, 0.60), 0xB1D16E);
    check_int("biome tropical", ns_biome_color(0.60, 0.60, 0.70), 0x427B19);

    check_int("humidity dryest", ns_humidity_color(0.10), 0xFF8B11);
    check_int("humidity dry", ns_humidity_color(0.30), 0x50FF00);
    check_int("humidity wet", ns_humidity_color(0.60), 0x55FFFF);
    check_int("humidity wetter", ns_humidity_color(0.80), 0x1446FF);
    check_int("humidity wettest", ns_humidity_color(2.0), 0x000064);
    check_int("humidity over", ns_humidity_color(20.0), 0x000000);

    check_int("heat coldest", ns_heat_color(0.03), 0x00FFFF);
    check_int("heat colder", ns_heat_color(0.10), 0xAAFFFF);
    check_int("heat cold", ns_heat_color(0.30), 0x00E585);
    check_int("heat hot", ns_heat_color(0.45), 0xFFFF64);
    check_int("heat hotter", ns_heat_color(0.60), 0xFF6400);
    check_int("heat hottest", ns_heat_color(0.90), 0xF10C00);

    check_int("resource none", ns_resource_color(0), 0x000000);
    check_int("resource silver", ns_resource_color(1), 0xC0C0C0);
    check_int("resource copper", ns_resource_color(2), 0xA47449);
    check_int("resource other", ns_resource_color(5), 0x000000);

    check_int("gray 0.0", ns_height_gray(0.0), 0x000000);
    check_int("gray 0.5", ns_height_gray(0.5), 0x808080);
    check_int("gray 1.0", ns_height_gray(1.0), 0xFFFFFF);
    check_int("level 20", ns_level_color(20), 0x7F0000);
    check_int("level 10", ns_level_color(10), 0xBF8080);

    if (ns_fail) {
        printf("SELFTEST FAILED\n");
        return 1;
    }
    printf("SELFTEST OK (permutation + simplex + planet cell + color parity)\n");
    return 0;
}
#endif
