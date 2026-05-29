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
        assert_equal(testOb->query_valid_name("abcd"), 1), // exactly 4 chars (minimum)
        assert_equal(testOb->query_valid_name("abcdefghijklmnopqr"), 1), // exactly 18 chars (maximum)

        assert_equal(testOb->query_valid_name(""), 0),
        assert_equal(testOb->query_valid_name("abc"), 0), // too short
        assert_equal(testOb->query_valid_name("abcdefghijklmnopqrstuvwxyz"), 0), // too long
        assert_equal(testOb->query_valid_name("!@#$%^&*()"), 0), // invalid chars
        assert_equal(testOb->query_valid_name("john-"), 0), // trailing hyphen
        assert_equal(testOb->query_valid_name("john "), 0), // trailing space
    }) :));
}

void test_save_path () {
    expect("query_save_path handles valid names", (: ({
        assert_regex(testOb->query_save_path("name", "character"), "^/save/character/n/name/character"),
        assert_regex(testOb->query_save_path("name", "pet"), "^/save/character/n/name/pet"),
        assert_regex(testOb->query_save_path("tester"), "^/save/character/t/tester/character.o"),
    }) :));

    expect("query_save_path handles invalid names", (: ({
        assert_equal(testOb->query_save_path(""), 0),
        assert_equal(testOb->query_save_path("no"), 0),
        assert_equal(testOb->query_save_path("nonononononononononononononononononononononononononononononononom"), 0),
        assert_equal(testOb->query_save_path("../../evil"), 0), // path traversal
    }) :));

    expect("query_save_path handles invalid types", (: ({
        assert_equal(testOb->query_save_path("name", "invalid"), 0),
        assert_equal(testOb->query_save_path("name", "../../evil"), 0),
    }) :));
}

void test_exists () {
    expect("query_exists handles invalid characters", (: ({
        assert_equal(testOb->query_exists(""), 0),
        assert_equal(testOb->query_exists("", "character"), 0),

        assert_equal(testOb->query_exists("testcharacterinvalid"), 0),
        assert_equal(testOb->query_exists("testcharacterinvalid", "character"), 0),
    }) :));

    expect("query_exists handles invalid types", (: ({
        assert_equal(testOb->query_exists("charactertest", "invalid"), 0),
        assert_equal(testOb->query_exists("charactertest", "../../evil"), 0),
    }) :));

    mkdirs("/save/character/c/charactertest");
    write_file("/save/character/c/charactertest/character.o", "charactertest.o mock save", 1);

    expect("query_exists handles valid characters", (: ({
        assert_equal(testOb->query_exists("charactertest"), 1),
        assert_equal(testOb->query_exists("Charactertest"), 1),
    }) :));

    rm("/save/character/c/charactertest/character.o");
    rmdir("/save/character/c/charactertest");
}

nosave private object ____TestCharacter;
void test_query_character () {
    expect("query_character returns 0 when character does not exist", (: ({
        assert_equal(testOb->query_character("charactertest"), 0),
    }) :));

    mkdirs("/save/character/c/charactertest");
    write_file("/save/character/c/charactertest/character.o", "charactertest.o mock save", 1);

    expect("query_character returns loaded character", (: ({
        assert_regex(__TestCharacter = testOb->query_character("charactertest"), "/std/character#"),
    }) :));

    rm("/save/character/c/charactertest/character.o");
    rmdir("/save/character/c/charactertest");
    if (__TestCharacter) destruct(__TestCharacter);
    __TestCharacter = 0;
}

void test_query_immortal () {
    expect("query_immortal returns 0 when character does not exist", (: ({
        assert_equal(testOb->query_immortal("charactertest"), 0),
    }) :));

    mkdirs("/save/character/c/charactertest");
    write_file("/save/character/c/charactertest/character.o", "__Immortal 1", 1);

    expect("query_immortal returns truthy for immortal character", (: ({
        assert_equal(testOb->query_immortal("charactertest"), 1),
    }) :));

    rm("/save/character/c/charactertest/character.o");
    rmdir("/save/character/c/charactertest");
}