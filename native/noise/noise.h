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

/* Port of D_PLANET query_noise. Computes the full biome-shaping result for one
   cell (out5) or a whole row (out, 5 doubles/cell):
   [0]=level [1]=height [2]=humidity [3]=heat [4]=resource. */
void ns_planet_cell(const ns_perm_t *perm, int size, int x, int y,
                    double heightFactor, double humidityFactor,
                    double heatFactor, double now, double *out5);
void ns_planet_row(const ns_perm_t *perm, int size, int y,
                   double heightFactor, double humidityFactor,
                   double heatFactor, double now, double *out);

/* Render a whole planet to an RGB PNG at path (pathlen bytes, not required to
   be NUL-terminated). Layout matches tools/planet-render/print-planet.js:
   a size*2 x size*4 grid of biome/height/humidity/heat/level/resource panels
   plus structure markers. Returns 1 on success, 0 on failure. */
int ns_planet_png(const ns_perm_t *perm, int size, double heightFactor,
                  double humidityFactor, double heatFactor, double now,
                  const char *path, int pathlen);

#endif
