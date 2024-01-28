int times = 1000;

string *seeds = ({ "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto", });

void measure_perlin () {
    int timeP = 0, time2d = 0, time3d = 0, pre = 0, post = 0;
    float x, y, z;
    string seed;
    int *p;
    float total2d = 0.0, total3d = 0.0;

    for (int i = 0; i < times; i ++) {
        seed = seeds[i % 9];
        x = random(1000) / 1000.0;
        y = random(1000) / 1000.0;
        z = random(1000) / 1000.0;

        pre = time_ns();
        p = noise_generate_permutation(seed);
        post = time_ns();
        timeP += (post - pre);

        pre = time_ns();
        total2d += "/secure/sefun/noise"->noise_perlin_2d(x, y, p, 8, 15.0);
        post = time_ns();
        time2d += (post - pre);

        pre = time_ns();
        total3d += "/secure/sefun/noise"->noise_perlin_3d(x, y, z, p, 8, 15.0);
        post = time_ns();
        time3d += (post - pre);
    }

    write("perlin - " + times + " calls - 2d avg=" + total2d/times + " - 3d avg=" + total3d/times + "\n");
    write(sprintf("%4s: ", "perm") + "%^ORANGE%^" + sprintf("%10.6f", timeP / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeP / 1000000.0) + " ms%^RESET%^\n");
    write(sprintf("%4s: ", "2D") + "%^ORANGE%^" + sprintf("%10.6f", time2d / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", time2d / 1000000.0) + " ms%^RESET%^\n");
    write(sprintf("%4s: ", "3D") + "%^ORANGE%^" + sprintf("%10.6f", time3d / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", time3d / 1000000.0) + " ms%^RESET%^\n");
}

void measure_simplex () {
    int timeP = 0, time4d = 0, pre = 0, post = 0;
    float x, y, z, w;
    string seed;
    mapping p;
    float total4d = 0.0;

    for (int i = 0; i < times; i ++) {
        seed = seeds[i % 9];
        x = random(1000) / 1000.0;
        y = random(1000) / 1000.0;
        z = random(1000) / 1000.0;
        w = random(1000) / 1000.0;

        pre = time_ns();
        p = noise_generate_permutation_simplex(seed);
        post = time_ns();
        timeP += (post - pre);

        pre = time_ns();
        total4d += "/secure/sefun/noise"->noise_simplex_4d(x, y, z, w, p, 8, 3.0);
        post = time_ns();
        time4d += (post - pre);
    }

    write("simplex - " + times + " calls - 4d avg=" + total4d/times + "\n");
    write(sprintf("%4s: ", "perm") + "%^ORANGE%^" + sprintf("%10.6f", timeP / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeP / 1000000.0) + " ms%^RESET%^\n");
    write(sprintf("%4s: ", "4D") + "%^ORANGE%^" + sprintf("%10.6f", time4d / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", time4d / 1000000.0) + " ms%^RESET%^\n");
}

void command (string input, mapping flags) {
    measure_perlin();
    measure_simplex();
}