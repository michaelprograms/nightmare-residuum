inherit M_TEST;

string *test_order () {
    return ({
        "test_applies",
        "test_startup_applies",
        "test_build_applies",
        "test_error_applies",
        // "test_ed_applies",
        "test_security_applies",
     });
}

object userOb;
mapping mudStats;
void test_applies () {
    expect("connect returns a valid user object", (: ({
        assert_regex(file_name(userOb = testOb->connect(0)), STD_USER[0..<3]+"#[0-9]+"),
        assert_equal(userOb->query_character(), 0),
        assert_equal(userOb->query_shell(), 0),
        assert_equal(destruct(userOb), 0),
    }) :));

    expect("get_mud_stats returns stats", (: ({
        assert_equal(mapp(mudStats = testOb->get_mud_stats()), 1),
        assert_equal(mudStats["NAME"], mud_name()),
        assert_equal(member_array("PLAYERS", keys(mudStats)) > -1, 1),
        assert_equal(member_array("UPTIME", keys(mudStats)) > -1, 1),
    }) :));
}

void test_startup_applies () {
    expect("epilog returns preload array", (: ({
        assert_equal(typeof(testOb->epilog(0)), "array"),
        assert_equal(sizeof(testOb->epilog(0)) > 0, 1),
        assert_equal(sizeof(testOb->epilog(1)) > 0, 0),
    }) :));
}

void test_build_applies () {
    expect("get_include_path handles paths", (: ({
        assert_equal(testOb->get_include_path("/test/Test"), ({ ":DEFAULT:" })),
        assert_equal(testOb->get_include_path("/std/module"), ({ ":DEFAULT:" })),
        assert_equal(testOb->get_include_path("/secure/module"), ({ ":DEFAULT:" })),

        assert_equal(testOb->get_include_path("/domain/Name"), ({ ":DEFAULT:", "/domain/Name/include" })),
        assert_equal(testOb->get_include_path("/domain/Name/test"), ({ ":DEFAULT:", "/domain/Name/include" })),
        assert_equal(testOb->get_include_path("/domain/Name/test/dir"), ({ ":DEFAULT:", "/domain/Name/include" })),

        assert_equal(testOb->get_include_path("/realm/Name"), ({ ":DEFAULT:", "/realm/Name/include" })),
        assert_equal(testOb->get_include_path("/realm/Name/test"), ({ ":DEFAULT:", "/realm/Name/include" })),
        assert_equal(testOb->get_include_path("/realm/Name/test/dir"), ({ ":DEFAULT:", "/realm/Name/include" })),
    }) :));
}

void test_error_applies () {

}

// used for retrieve_ed_setup and save_ed_setup
private int __EdConfig = 0;
int query_ed_setup () {
    return __EdConfig;
}
void set_ed_setup (int config) {
    __EdConfig = config;
}

void test_ed_applies () {
    expect("retrieve_ed_setup", (: ({
        assert_equal(testOb->retrieve_ed_setup(this_object()), 0),
        assert_equal(testOb->save_ed_setup(this_object(), 123), 1),
        assert_equal(testOb->retrieve_ed_setup(this_object()), 123),
    }) :));
}

object basicOb;
void test_security_applies () {
    expect("valid_override handles requests", (: ({
        assert_equal(testOb->valid_override("/secure/sefun/override"), 1),
        assert_equal(testOb->valid_override("/insecure"), 0),
        assert_equal(testOb->valid_override("/std/user/input", "input_to"), 1),
        assert_equal(testOb->valid_override("/std/user/input", "get_char"), 1),
    }) :));

    expect("valid_database handles calls", (: ({
        assert_equal(testOb->valid_database(0, 0, 0), 1),
    }) :));

    expect("valid_read handles calls", (: ({
        assert_regex(file_name(basicOb = new(STD_OBJECT)), STD_OBJECT[0..<3]+"#[0-9]+"),

        assert_equal(testOb->valid_read(0, 0, 0), 0),
        assert_equal(testOb->valid_read(0, testOb, "read_file"), 0),

        assert_equal(testOb->valid_read("/", testOb, "read_file"), 1),
        assert_equal(testOb->valid_read("/tmp/void/doesntexist", basicOb, "read_file"), 0),
    }) :));

    expect("valid_write handles calls", (: ({
        assert_equal(testOb->valid_write("/", testOb, "write_file"), 1),
        assert_equal(testOb->valid_write("/save", basicOb, "write_file"), 0),
        assert_equal(destruct(basicOb), 0),
    }) :));
}

void test_parsing_applies () {
    expect("parse_command_id_list returns list of nouns", (: ({
        assert_equal(implode(testOb->parse_command_id_list(), ","), "thing"),
    }) :));

    expect("parse_command_adjectiv_id_list returns list of adjectives", (: ({
        assert_equal(implode(testOb->parse_command_adjectiv_id_list(), ","), "a,an,the"),
    }) :));

    expect("parse_command_plural_id_list returns list of plurals", (: ({
        assert_equal(implode(testOb->parse_command_plural_id_list(), ","), "things,them,everything"),
    }) :));

    expect("parse_command_prepos_list returns list of prepositions", (: ({
        assert_equal(sizeof(testOb->parse_command_prepos_list()), 50),
    }) :));

    expect("parse_command_all_word returns all word", (: ({
        assert_equal(testOb->parse_command_all_word(), "all"),
    }) :));
}
