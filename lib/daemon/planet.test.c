#include "planet.h"

inherit M_TEST;

void test_biome_colors () {
    expect("biome ANSI colors returned", (: ({
        assert_equal(testOb->query_biome_color_ansi("invalid"), "\e[38;2;128;0;0m"),
        assert_equal(testOb->query_biome_color_ansi("ice"), "\e[38;2;255;255;255m"),
    }) :));
    expect("biome hex colors returned", (: ({
        assert_equal(testOb->query_biome_color_hex("invalid"), "#800000"),
        assert_equal(testOb->query_biome_color_hex("ice"), "#FFFFFF"),
    }) :));

    expect("humidity hex colors returned", (: ({
        assert_equal(testOb->query_humidity_color_hex(UNDEFINED), "#F10C00"),
        assert_equal(testOb->query_humidity_color_hex(HUMIDITY_DRYEST), "#FF8B11"),
        assert_equal(testOb->query_humidity_color_hex(HUMIDITY_WETTER), "#1446FF"),
    }) :));

    expect("heat hex colors returned", (: ({
        assert_equal(testOb->query_heat_color_hex(UNDEFINED), "#F10C00"),
        assert_equal(testOb->query_heat_color_hex(HEAT_COLDEST), "#00FFFF"),
        assert_equal(testOb->query_heat_color_hex(HEAT_HOTTER), "#FF6400"),
    }) :));

    expect("resource hex colors returned", (: ({
        assert_equal(testOb->query_resource_color_hex(UNDEFINED), "#000000"),
        assert_equal(testOb->query_resource_color_hex(1), "#C0C0C0"),
        assert_equal(testOb->query_resource_color_hex(2), "#A47449"),
    }) :));
}
void test_humidity () {
    expect("humidity ANSI colors returned", (: ({
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_COLDEST, HUMIDITY_DRYEST), "frozen water"),
    }) :));
}

void test_biome () {
    expect("biomes are returned", (: ({
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_COLDEST, HUMIDITY_DRYEST), "frozen water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_COLDER, HUMIDITY_DRYEST), "icy water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_COLD, HUMIDITY_DRYEST), "deeper water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_HOT, HUMIDITY_DRYEST), "deeper water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_HOTTER, HUMIDITY_DRYEST), "deeper water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEPER, HEAT_HOTTEST, HUMIDITY_DRYEST), "deeper water"),

        assert_equal(testOb->query_biome(HEIGHT_DEEP, HEAT_COLDEST, HUMIDITY_DRYEST), "frozen water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEP, HEAT_COLDER, HUMIDITY_DRYEST), "icy water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEP, HEAT_COLD, HUMIDITY_DRYEST), "deep water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEP, HEAT_HOT, HUMIDITY_DRYEST), "deep water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEP, HEAT_HOTTER, HUMIDITY_DRYEST), "deep water"),
        assert_equal(testOb->query_biome(HEIGHT_DEEP, HEAT_HOTTEST, HUMIDITY_DRYEST), "deep water"),

        assert_equal(testOb->query_biome(HEIGHT_SHALLOW, HEAT_COLDEST, HUMIDITY_DRYEST), "frozen water"),
        assert_equal(testOb->query_biome(HEIGHT_SHALLOW, HEAT_COLDER, HUMIDITY_DRYEST), "icy water"),
        assert_equal(testOb->query_biome(HEIGHT_SHALLOW, HEAT_COLD, HUMIDITY_DRYEST), "shallow water"),
        assert_equal(testOb->query_biome(HEIGHT_SHALLOW, HEAT_HOT, HUMIDITY_DRYEST), "shallow water"),
        assert_equal(testOb->query_biome(HEIGHT_SHALLOW, HEAT_HOTTER, HUMIDITY_DRYEST), "shallow water"),
        assert_equal(testOb->query_biome(HEIGHT_SHALLOW, HEAT_HOTTEST, HUMIDITY_DRYEST), "shallow water"),

        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLDEST, HUMIDITY_DRYEST), "ice"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLDEST, HUMIDITY_WETTER), "ice"),

        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLDER, HUMIDITY_DRYEST), "tundra"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLDER, HUMIDITY_WETTER), "tundra"),

        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLD, HUMIDITY_DRYER), "grassland"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLD, HUMIDITY_DRY), "woodland"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_COLD, HUMIDITY_WET), "boreal forest"),

        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOT, HUMIDITY_DRYER), "desert"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOT, HUMIDITY_DRY), "woodland"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOT, HUMIDITY_WET), "temperate rainforest"),

        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOTTER, HUMIDITY_DRYER), "desert"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOTTER, HUMIDITY_WET), "savanna"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOTTER, HUMIDITY_WETTER), "tropical rainforest"),

        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOTTEST, HUMIDITY_DRYER), "desert"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOTTEST, HUMIDITY_WET), "savanna"),
        assert_equal(testOb->query_biome(HEIGHT_SHORE, HEAT_HOTTEST, HUMIDITY_WETTER), "tropical rainforest"),
    }) :));
}