inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/color.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_hex_to_int () {
    expect_function("hex_to_int", testOb);

    expect("hex_to_int handled base 16 to base 10", (: ({
        assert(testOb->hex_to_int("00"), "==", 0),
        assert(testOb->hex_to_int("0A"), "==", 10),
        assert(testOb->hex_to_int("0F"), "==", 15),
        assert(testOb->hex_to_int("A0"), "==", 160),
        assert(testOb->hex_to_int("F0"), "==", 240),
        assert(testOb->hex_to_int("AA"), "==", 170),
        assert(testOb->hex_to_int("BB"), "==", 187),
        assert(testOb->hex_to_int("CC"), "==", 204),
        assert(testOb->hex_to_int("DD"), "==", 221),
        assert(testOb->hex_to_int("EE"), "==", 238),
        assert(testOb->hex_to_int("FF"), "==", 255),
        assert(testOb->hex_to_int("0a"), "==", 10),
        assert(testOb->hex_to_int("0f"), "==", 15),
        assert(testOb->hex_to_int("a0"), "==", 160),
        assert(testOb->hex_to_int("f0"), "==", 240),
        assert(testOb->hex_to_int("aa"), "==", 170),
        assert(testOb->hex_to_int("bb"), "==", 187),
        assert(testOb->hex_to_int("cc"), "==", 204),
        assert(testOb->hex_to_int("dd"), "==", 221),
        assert(testOb->hex_to_int("ee"), "==", 238),
        assert(testOb->hex_to_int("ff"), "==", 255),
        assert(testOb->hex_to_int("-a"), "==", -10),
        assert(testOb->hex_to_int("a"), "==", 10),
        assert(testOb->hex_to_int("A"), "==", 10),
        assert(testOb->hex_to_int("-A"), "==", -10),
        assert(testOb->hex_to_int("FFF"), "==", 4095),
    }) :));

    expect("hex_to_int bandled bad inputs", (: ({
        assert((: testOb->hex_to_int() :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
        assert((: testOb->hex_to_int(0) :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
        assert((: testOb->hex_to_int("") :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
        assert((: testOb->hex_to_int(({})) :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
        assert((: testOb->hex_to_int(([])) :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
        assert((: testOb->hex_to_int("GG") :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
        assert((: testOb->hex_to_int("#A") :), "catch", "*Bad argument 1 to color->hex_to_int\n"),
    }) :));
}

private int *Color;
void test_random_color () {
    expect_function("query_random_color", testOb);

    Color = testOb->query_random_color();
    expect("query_random_color returns random triplets", (: ({
        assert(sizeof(Color), "==", 3),
        // triplets are unique
        assert(Color[0] != Color[1], "==", 1),
        assert(Color[1] != Color[2], "==", 1),
    }) :));
}

void test_sRGB () {
    expect_function("color_to_sRGB", testOb);
    expect_function("color_from_sRGB", testOb);

    expect("converting to sRGB behaves", (: ({
        assert(testOb->color_to_sRGB(0.0), "==", 0),
        assert(testOb->color_to_sRGB(0.25), "==", 137),
        assert(testOb->color_to_sRGB(0.5), "==", 188),
        assert(testOb->color_to_sRGB(0.75), "==", 225),
        assert(testOb->color_to_sRGB(1.0), "==", 255),
    }) :));
    expect("color_to_sRGB handles bad argument 1", (: ({
        assert((: testOb->color_to_sRGB() :), "catch", "*Bad argument 1 to color->color_to_sRGB\n"),
        assert((: testOb->color_to_sRGB("") :), "catch", "*Bad argument 1 to color->color_to_sRGB\n"),
        assert((: testOb->color_to_sRGB(1) :), "catch", "*Bad argument 1 to color->color_to_sRGB\n"),
        assert((: testOb->color_to_sRGB(({ })) :), "catch", "*Bad argument 1 to color->color_to_sRGB\n"),
        assert((: testOb->color_to_sRGB(([ ])) :), "catch", "*Bad argument 1 to color->color_to_sRGB\n"),
        assert((: testOb->color_to_sRGB((: 1 :)) :), "catch", "*Bad argument 1 to color->color_to_sRGB\n"),
    }) :));

    expect("converting from sRGB behaves", (: ({
        assert(testOb->color_from_sRGB(0), "==", 0.0),
        assert(testOb->color_from_sRGB(137), "==", 0.25),
        assert(testOb->color_from_sRGB(188), "==", 0.5),
        assert(testOb->color_from_sRGB(225), "==", 0.75),
        assert(testOb->color_from_sRGB(255), "==", 1.0),
    }) :));
    expect("color_from_sRGB handles bad argument 1", (: ({
        assert((: testOb->color_from_sRGB() :), "catch", "*Bad argument 1 to color->color_from_sRGB\n"),
        assert((: testOb->color_from_sRGB("") :), "catch", "*Bad argument 1 to color->color_from_sRGB\n"),
        assert((: testOb->color_from_sRGB(1.0) :), "catch", "*Bad argument 1 to color->color_from_sRGB\n"),
        assert((: testOb->color_from_sRGB(({ })) :), "catch", "*Bad argument 1 to color->color_from_sRGB\n"),
        assert((: testOb->color_from_sRGB(([ ])) :), "catch", "*Bad argument 1 to color->color_from_sRGB\n"),
        assert((: testOb->color_from_sRGB((: 1 :)) :), "catch", "*Bad argument 1 to color->color_from_sRGB\n"),
    }) :));
}