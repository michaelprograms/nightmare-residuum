#include "planet.h"

inherit M_TEST;

/**
 * @var {"/daemon/planet"} testOb
 */

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
        assert_equal(testOb->query_humidity_color_hex(UNDEFINED), "#000000"),
        assert_equal(testOb->query_humidity_color_hex(HUMIDITY_DRYEST), "#FF8B11"),
        assert_equal(testOb->query_humidity_color_hex(HUMIDITY_WETTER), "#1446FF"),
    }) :));

    expect("heat hex colors returned", (: ({
        assert_equal(testOb->query_heat_color_hex(UNDEFINED), "#000000"),
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

void test_noise () {
    mapping p = noise_generate_permutation_simplex("test");
    expect("noise initializes all values", (: ({
        // UNDEFINED values report different then defined values (different than defaults)
        assert_equal(testOb->query_noise($(p), 100, 49, 49, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED) != testOb->query_noise($(p), 100, 49, 49, 0.1, 0.1, 0.1, 1717171717), 1),
    }) :));

    expect("noise ensures central land mass", (: ({
        assert_equal(testOb->query_noise($(p), 100, 49, 49, 0.1, 0.1, 0.1, 1717171717), ([ "heat": 0.068189, "height": 0.500000, "humidity": 0.156711, "level": 1, "resource": 0 ])),
    }) :));

    expect("noise returns different values for x,y coordinates", (: ({
        assert_equal(testOb->query_noise($(p), 100, 49, 49, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.811243, "height": 0.638848, "humidity": 0.630995, "level": 1, "resource": 0 ])),
        assert_equal(testOb->query_noise($(p), 100, 49, 50, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.792699, "height": 0.619084, "humidity": 0.515993, "level": 1, "resource": 9 ])),
        assert_equal(testOb->query_noise($(p), 100, 50, 49, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.879001, "height": 0.600883, "humidity": 0.648935, "level": 1, "resource": 5 ])),
        assert_equal(testOb->query_noise($(p), 100, 50, 50, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.901959, "height": 0.547387, "humidity": 0.593906, "level": 1, "resource": 1 ])),
    }) :));

    expect("noise returns water adjustments", (: ({
        // water - lake
        assert_equal(testOb->query_noise($(p), 100, 70, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.400000, "humidity": 2.000008, "level": 20, "resource": 0 ])),
        // water - height below height shallow
        assert_equal(testOb->query_noise($(p), 100, 8, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.353097, "humidity": 2.135435, "level": 20, "resource": 0 ])),
        // water - height above height shallow
        assert_equal(testOb->query_noise($(p), 100, 14, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.400000, "humidity": 1.924181, "level": 20, "resource": 0 ])),
    }) :));

    expect("noise returns humidity adjustments", (: ({
        // humidity - height below height deeper
        assert_equal(testOb->query_noise($(p), 100, 46, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.281879, "humidity": 2.366390, "level": 20, "resource": 0 ])),
        // humidity - height below height deep
        assert_equal(testOb->query_noise($(p), 100, 45, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.365770, "humidity": 2.399587, "level": 20, "resource": 0 ])),
        // humidity - height below height shallow
        assert_equal(testOb->query_noise($(p), 100, 44, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.416786, "humidity": 2.145491, "level": 20, "resource": 0 ])),
        // humidity - height below height shore
        assert_equal(testOb->query_noise($(p), 100, 38, 0, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.469673, "humidity": 0.530347, "level": 20, "resource": 0 ])),
    }) :));

    expect("noise returns heat adjustments", (: ({
        // north 40%
        assert_equal(testOb->query_noise($(p), 100, 0, 99, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.559112, "humidity": 0.633468, "level": 20, "resource": 3 ])),
        // center 20%
        assert_equal(testOb->query_noise($(p), 100, 95, 50, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.529401, "height": 0.557625, "humidity": 0.355702, "level": 18, "resource": 4 ])),
        // south 40%
        assert_equal(testOb->query_noise($(p), 100, 99, 99, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.000000, "height": 0.559822, "humidity": 0.642394, "level": 20, "resource": 6 ])),

        // heat capped at 1.0
        assert_equal(testOb->query_noise($(p), 100, 49, 45, UNDEFINED, UNDEFINED, UNDEFINED, 1717171717), ([ "heat": 0.866250, "height": 0.654389, "humidity": 0.382916, "level": 2, "resource": 9 ])),
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

void test_querying_planets () {
    expect("querying all is an array", (: ({
        assert_equal(arrayp(testOb->query_all_planets()), 1),
    }) :));

    expect("querying default planet (Terra)", (: ({
        assert_equal(mapp(testOb->query_planet("Terra")), 1),
        assert_equal(testOb->query_planet("Terra")["name"], "Terra"),
        assert_equal(testOb->query_planet_size("Terra"), 500),
    }) :));
}

void test_creating_and_adjusting_planet (function done) {
    string testPlanet = "test_" + time();
    string testPlanetFile = "/save/planet/t/"+testPlanet+".o";
    expect("create_planet behaves", (: ({
        // planet doesn't exist yet
        assert_equal(file_size($(testPlanetFile)), -1),
        assert_equal(testOb->query_planet($(testPlanet)), ([ ])),
        // create test planet
        assert_equal(testOb->create_planet($(testPlanet), ([ "size": 123 ])), 1),
        // planet exists
        assert_equal(testOb->query_planet($(testPlanet)), ([ "name": $(testPlanet), "size": 123 ])),
        // planet was already created
        assert_equal(testOb->create_planet($(testPlanet), ([ ])), 0),
    }) :));
    expect("adjust_planet behaves", (: ({
        // adjust planet size
        assert_equal(testOb->adjust_planet($(testPlanet), ([ "size": 321 ])), 1),
        // can't adjust non-existent planet
        assert_equal(testOb->adjust_planet($(testPlanet+"-bad"), ([ "size": 321 ])), 0),
    }) :));

    testOb->adjust_planet(testPlanet, ([ "size": 10 ]));
    testOb->generate_json(testPlanet, UNDEFINED);
    call_out_walltime(function (function done, string testPlanet, string testPlanetFile) {
        expect("planet is generated", (: ({
            assert_equal(file_size("/tmp/"+$(testPlanet)+".json") > 0, 1),
            rm($(testPlanetFile)),
            assert_equal(file_size($(testPlanetFile)), -1),
        }) :));

        evaluate(done);
    }, 0.1, done, testPlanet, testPlanetFile);
}