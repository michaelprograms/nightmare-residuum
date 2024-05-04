inherit M_TEST;

void test_biome_colors () {
    expect("ansi colors returned", (: ({
        assert_equal(testOb->query_biome_color_ansi("invalid"), "\e[38;2;128;0;0m"),

        assert_equal(testOb->query_biome_color_ansi("ice"), "\e[38;2;255;255;255m"),
    }) :));

    expect("hex colors returned", (: ({
        assert_equal(testOb->query_biome_color_hex("invalid"), "#800000"),

        assert_equal(testOb->query_biome_color_hex("ice"), "#FFFFFF"),
    }) :));
}