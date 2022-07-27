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