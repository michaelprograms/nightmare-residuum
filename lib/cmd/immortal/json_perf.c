int times = 1000;

void measure_json () {
    int timeSefun = 0, timeSefun2 = 0;
    int pre = 0, post = 0;
    object *obs = objects();
    string str;

    for (int i = 0; i < times; i ++) {
        pre = time_ns();
        str = "/secure/sefun/json"->json_encode(obs);
        post = time_ns();
        timeSefun += (post - pre);

        pre = time_ns();
        "/secure/sefun/json"->json_decode(str);
        post = time_ns();
        timeSefun2 += (post - pre);
    }

    write("array size being used: "+sizeof(obs)+"\n");
    write("json_encode/json_decode - " + times + "x\n");
    write(sprintf("%16s: ", "sefun encode") + "%^ORANGE%^" + sprintf("%10.6f", timeSefun / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeSefun / 1000000.0) + " ms%^RESET%^\n");
    write(sprintf("%16s: ", "sefun decode") + "%^ORANGE%^" + sprintf("%10.6f", timeSefun2 / 1000000.0 / times) + " ms%^RESET%^ x " + sprintf("%d", times) + " = %^ORANGE%^" + sprintf("%f", timeSefun2 / 1000000.0) + " ms%^RESET%^\n");
}

void command (string input, mapping flags) {
    measure_json();
}