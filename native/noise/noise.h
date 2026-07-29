#ifndef NR_NOISE_H
#define NR_NOISE_H

#include <stdint.h>

typedef struct {
    int32_t p[256];
    int32_t gx[256];
    int32_t gy[256];
    int32_t gz[256];
    int32_t gw[256];
} ns_perm_t;

/* Build the seeded permutation + resolved 4D gradients into *out.
   Port of LPC noise_generate_permutation[_simplex] (lib/secure/sefun/noise.c). */
void ns_permutation(const uint8_t *seed, int seedlen, ns_perm_t *out);

/* Fractal 4D simplex noise. Port of LPC noise_simplex_4d. */
double ns_simplex_4d(double x, double y, double z, double w,
                     const ns_perm_t *perm, int octaves, double scale);

/* Fill count samples in one call: x = x0 + k*dx, k in [0,count). */
void ns_simplex_4d_row(double *out, int count,
                       double x0, double dx, double y, double z, double w,
                       const ns_perm_t *perm, int octaves, double scale);

#endif
