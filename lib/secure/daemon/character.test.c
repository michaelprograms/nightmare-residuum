inherit M_TEST;

/**
 * @var {"/secure/daemon/character"} testOb
 */

void test_valid_name () {
    expect("query_valid_name handles names", (: ({
        assert_equal(testOb->query_valid_name("valid"), 1),
        assert_equal(testOb->query_valid_name("abcdefghijklm"), 1),
        assert_equal(testOb->query_valid_name("nopqrstuvwxyz"), 1),
        assert_equal(testOb->query_valid_name("a'b c-d"), 1), // valid decorators

        assert_equal(testOb->query_valid_name(""), 0),
        assert_equal(testOb->query_valid_name("abc"), 0), // too short
        assert_equal(testOb->query_valid_name("abcdefghijklmnopqrstuvwxyz"), 0), // too long
        assert_equal(testOb->query_valid_name("!@#$%^&*()"), 0), // invalid chars
    }) :));
}

void test_save_path () {
    expect("query_save_path handles valid names", (: ({
        assert_regex(testOb->query_save_path("name", "type"), "^/save/character/n/name/type"),
        assert_regex(testOb->query_save_path("tester"), "^/save/character/t/tester/character.o"),
    }) :));

    expect("query_save_path handles invalid names", (: ({
        assert_equal(testOb->query_save_path(""), 0),
        assert_equal(testOb->query_save_path("no"), 0),
        assert_equal(testOb->query_save_path("nonononononononononononononononononononononononononononononononom"), 0),
    }) :));
}

void test_exists () {
    expect("query_exists handles invalid characters", (: ({
        assert_equal(testOb->query_exists(""), 0),
        assert_equal(testOb->query_exists("", "character"), 0),

        assert_equal(testOb->query_exists("testcharacterinvalid"), 0),
        assert_equal(testOb->query_exists("testcharacterinvalid", "character"), 0),
    }) :));

    expect("query_exists handles valid characters", (: ({
        // create mock character file
        assert_equal(mkdirs("/save/character/c/charactertest"), 1),
        assert_equal(write_file("/save/character/c/charactertest/character.o", "charactertest.o mock save", 1), 1),

        // test query_exists
        assert_equal(testOb->query_exists("charactertest"), 1),

        // delete mock character file
        assert_equal(rm("/save/character/c/charactertest/character.o"), 1),
        assert_equal(rmdir("/save/character/c/charactertest"), 1),
    }) :));
}

object TestCharacter;
void test_load_character () {
    expect("query_character returns loaded character", (: ({
        // character doesn't exist
        assert_equal(testOb->query_character("charactertest"), 0),

        // create mock character file
        assert_equal(mkdirs("/save/character/c/charactertest"), 1),
        assert_equal(write_file("/save/character/c/charactertest/character.o", "charactertest.o mock save", 1), 1),

        // test query_character
        assert_regex(TestCharacter = testOb->query_character("charactertest"), "/std/character#"),

        // delete mock character file
        assert_equal(rm("/save/character/c/charactertest/character.o"), 1),
        assert_equal(rmdir("/save/character/c/charactertest"), 1),
    }) :));
    if (TestCharacter) destruct(TestCharacter);

    expect("query_immortal returns returns truthy", (: ({
        // character isn't immortal / doesn't exist
        assert_equal(testOb->query_immortal("charactertest"), 0),

        // create mock character file
        assert_equal(mkdirs("/save/character/c/charactertest"), 1),
        assert_equal(write_file("/save/character/c/charactertest/character.o", "__Immortal 1", 1), 1),

        // test query_immortal again
        assert_equal(testOb->query_immortal("charactertest"), 1),

        // delete mock character file
        assert_equal(rm("/save/character/c/charactertest/character.o"), 1),
        assert_equal(rmdir("/save/character/c/charactertest"), 1),
    }) :));
}