inherit M_TEST;

void test_hex_to_int () {
    expect("hex_to_int handled base 16 to base 10", (: ({
        assert_equal(testOb->hex_to_int("00"), 0),
        assert_equal(testOb->hex_to_int("0A"), 10),
        assert_equal(testOb->hex_to_int("0F"), 15),
        assert_equal(testOb->hex_to_int("A0"), 160),
        assert_equal(testOb->hex_to_int("F0"), 240),
        assert_equal(testOb->hex_to_int("AA"), 170),
        assert_equal(testOb->hex_to_int("BB"), 187),
        assert_equal(testOb->hex_to_int("CC"), 204),
        assert_equal(testOb->hex_to_int("DD"), 221),
        assert_equal(testOb->hex_to_int("EE"), 238),
        assert_equal(testOb->hex_to_int("FF"), 255),
        assert_equal(testOb->hex_to_int("0a"), 10),
        assert_equal(testOb->hex_to_int("0f"), 15),
        assert_equal(testOb->hex_to_int("a0"), 160),
        assert_equal(testOb->hex_to_int("f0"), 240),
        assert_equal(testOb->hex_to_int("aa"), 170),
        assert_equal(testOb->hex_to_int("bb"), 187),
        assert_equal(testOb->hex_to_int("cc"), 204),
        assert_equal(testOb->hex_to_int("dd"), 221),
        assert_equal(testOb->hex_to_int("ee"), 238),
        assert_equal(testOb->hex_to_int("ff"), 255),
        assert_equal(testOb->hex_to_int("-a"), -10),
        assert_equal(testOb->hex_to_int("a"), 10),
        assert_equal(testOb->hex_to_int("A"), 10),
        assert_equal(testOb->hex_to_int("-A"), -10),
        assert_equal(testOb->hex_to_int("FFF"), 4095),
    }) :));

    expect("hex_to_int catches bad input", (: ({
        assert_catch((: testOb->hex_to_int(UNDEFINED) :), "*Bad argument 1 to convert->hex_to_int\n"),
        assert_catch((: testOb->hex_to_int("") :), "*Bad argument 1 to convert->hex_to_int\n"),
    }) :));
}

void test_int_to_binary () {
    expect("int_to_binary handled base 10 to base 2", (: ({
        assert_equal(testOb->int_to_binary(0), "0"),
        assert_equal(testOb->int_to_binary(1), "1"),
        assert_equal(testOb->int_to_binary(2), "10"),
        assert_equal(testOb->int_to_binary(10), "1010"),
        assert_equal(testOb->int_to_binary(-1), "1111111111111111111111111111111111111111111111111111111111111111"),
    }) :));
}