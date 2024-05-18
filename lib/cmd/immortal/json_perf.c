inherit "/std/json.c";

int times = 1000;

void measure_json () {
    int timeSefun = 0, timeInherit = 0, pre = 0, post = 0;
    int timeSefun2 = 0;
    object *obs = objects(); // + objects() + objects();

    for (int i = 0; i < times; i ++) {
        pre = perf_counter_ns();
        "/secure/sefun/json"->json_encode(obs);
        post = perf_counter_ns();
        timeSefun += (post - pre);

        pre = perf_counter_ns();
        "/secure/sefun/json"->json_encode2(obs);
        post = perf_counter_ns();
        timeSefun2 += (post - pre);

        pre = perf_counter_ns();
        json::json_encode(obs);
        post = perf_counter_ns();
        timeInherit += (post - pre);
    }

    write("array size being used: "+sizeof(obs)+"\n");
    write("json_encode - " + times + "x\n");
    write(sprintf("%16s: ", "sefun adjusted") + "%^ORANGE%^" + sprintf("%10.6f", timeSefun / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeSefun / 1000000.0) + " ms%^RESET%^\n");
    write(sprintf("%16s: ", "std as sefun") + "%^ORANGE%^" + sprintf("%10.6f", timeSefun2 / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeSefun2 / 1000000.0) + " ms%^RESET%^\n");
    write(sprintf("%16s: ", "std as inherit") + "%^ORANGE%^" + sprintf("%10.6f", timeInherit / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeInherit / 1000000.0) + " ms%^RESET%^\n");
}

void command (string input, mapping flags) {
    measure_json();
}

/*
eval string json = json_encode(objects()); write("sefun: " + identify("/secure/sefun/json.c"->json_decode(json))+"\n"); write("std: " + identify("/std/json.c"->json_decode(json))+"\n");


eval string json = json_encode("test"); write("sefun: " + identify("/secure/sefun/json.c"->json_decode(json))+"\n"); write("std: " + identify("/std/json.c"->json_decode(json))+"\n");
*/