// inherit "/std/json.c";

int times = 1000;

void measure_json () {
    int timeSefun = 0, /*timeInherit = 0,*/ pre = 0, post = 0;
    object *obs = objects();

    for (int i = 0; i < times; i ++) {
        pre = perf_counter_ns();
        "/secure/sefun/json"->json_encode(obs);
        post = perf_counter_ns();
        timeSefun += (post - pre);

        // pre = perf_counter_ns();
        // json::json_encode(obs);
        // post = perf_counter_ns();
        // timeInherit += (post - pre);
    }

    write("json - " + times + "\n");
    write(sprintf("%10s: ", "sefun") + "%^ORANGE%^" + sprintf("%10.6f", timeSefun / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeSefun / 1000000.0) + " ms%^RESET%^\n");
    // write(sprintf("%10s: ", "inherit") + "%^ORANGE%^" + sprintf("%10.6f", timeInherit / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeInherit / 1000000.0) + " ms%^RESET%^\n");
}

void command (string input, mapping flags) {
    measure_json();
}