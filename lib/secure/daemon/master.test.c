inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/daemon/master");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_order () {
    return ({
        "test_applies",
        "test_startup_applies",
        "test_build_applies",
        "test_error_applies",
        // "test_ed_applies",
        "test_security_applies",
        "test_valid_applies",
     });
}

object userOb;
mapping mudStats;
void test_applies () {
    expect_function("connect", testOb);

    expect("connect returns a valid user object", (: ({
        assert(file_name(userOb = testOb->connect(0)), "regex", OBJ_USER+"#[0-9]+"),
        assert(userOb->query_account(), "==", 0),
        assert(userOb->query_character(), "==", 0),
        assert(userOb->query_shell(), "==", 0),
        assert(destruct(userOb), "==", 0),
    }) :));

    expect_function("get_mud_stats", testOb);

    expect("get_mud_stats returns stats", (: ({
        assert(mapp(mudStats = testOb->get_mud_stats()), "==", 1),
        assert(mudStats["NAME"], "==", mud_name()),
        assert(member_array("PLAYERS", keys(mudStats)) > -1, "==", 1),
        assert(member_array("UPTIME", keys(mudStats)) > -1, "==", 1),
    }) :));
}

void test_startup_applies () {
    expect_function("epilog", testOb);

    expect("epilog returns preload array", (: ({
        assert(typeof(testOb->epilog(0)), "==", "array"),
        assert(sizeof(testOb->epilog(0)) > 0, "==", 1),
        assert(sizeof(testOb->epilog(1)) > 0, "==", 0),
    }) :));

    expect_function("flag", testOb);

    expect_function("preload", testOb);
}

void test_build_applies () {
    expect_function("compile_object", testOb);
    // @TODO test compile_object here

    expect_function("object_name", testOb);

    expect_function("get_include_path", testOb);

    expect("get_include_path handles paths", (: ({
        assert(testOb->get_include_path("/test/Test"), "==", ({ ":DEFAULT:" })),
        assert(testOb->get_include_path("/std/module"), "==", ({ ":DEFAULT:" })),
        assert(testOb->get_include_path("/secure/module"), "==", ({ ":DEFAULT:" })),

        assert(testOb->get_include_path("/domain/Name"), "==", ({ ":DEFAULT:", "/domain/Name/include" })),
        assert(testOb->get_include_path("/domain/Name/test"), "==", ({ ":DEFAULT:", "/domain/Name/include" })),
        assert(testOb->get_include_path("/domain/Name/test/dir"), "==", ({ ":DEFAULT:", "/domain/Name/include" })),

        assert(testOb->get_include_path("/realm/Name"), "==", ({ ":DEFAULT:", "/realm/Name/include" })),
        assert(testOb->get_include_path("/realm/Name/test"), "==", ({ ":DEFAULT:", "/realm/Name/include" })),
        assert(testOb->get_include_path("/realm/Name/test/dir"), "==", ({ ":DEFAULT:", "/realm/Name/include" })),
    }) :));
}

void test_error_applies () {
    expect_function("crash", testOb);
    expect_function("error_handler", testOb);
    expect_function("log_error", testOb);
}

void test_ed_applies () {
//     expect_function("get_save_file_name", testOb);

    // make_absolute_path testing handled by sefun sanitize_path
    expect_function("make_path_absolute", testOb);

//     expect_function("retrieve_ed_setup", testOb);

//     expect_function("save_ed_setup", testOb);
}

void test_security_applies () {
    expect_function("privs_file", testOb);
    // privs_file testing handled by D_ACCESS query_file_privs

    expect_function("valid_override", testOb);

    expect("valid_override handles requests", (: ({
        assert(testOb->valid_override("/secure/sefun/override"), "==", 1),
        assert(testOb->valid_override("/insecure"), "==", 0),
        assert(testOb->valid_override("/secure/user/input", "input_to"), "==", 1),
        assert(testOb->valid_override("/secure/user/input", "get_char"), "==", 1),
    }) :));
}

object basicOb;
void test_valid_applies () {
    // expect_function("valid_bind", testOb);
    // expect_function("valid_database", testOb);
    // expect_function("valid_hide", testOb);
    // expect_function("valid_link", testOb);
    // expect_function("valid_object", testOb);

    expect_function("valid_read", testOb);

    expect("valid_read handles calls", (: ({
        assert(file_name(basicOb = new(STD_OBJECT)), "regex", STD_OBJECT[0..<3]+"#[0-9]+"),
        assert(testOb->valid_read("/", testOb, "read_file"), "==", 1),
        assert(testOb->valid_read("/tmp/void/doesntexist", basicOb, "read_file"), "==", 0),
    }) :));

    expect_function("valid_write", testOb);

    expect("valid_write handles calls", (: ({
        assert(testOb->valid_write("/", testOb, "write_file"), "==", 1),
        assert(testOb->valid_write("/save", basicOb, "write_file"), "==", 0),
        assert(destruct(basicOb), "==", 0),
    }) :));

    // expect_function("valid_save_binary", testOb);
    // expect_function("valid_seteuid", testOb);
    // expect_function("valid_shadow", testOb);

    expect_function("valid_socket", testOb);
    // @TODO test valid_socket

}

void test_parsing_applies () {
    expect_function("parse_command_id_list", testOb);
    expect("parse_command_id_list returns list of nouns", (: ({
        assert(implode(testOb->parse_command_id_list(), ","), "==", "thing"),
    }) :));

    expect_function("parse_command_adjectiv_id_list", testOb);
    expect("parse_command_adjectiv_id_list returns list of adjectives", (: ({
        assert(implode(testOb->parse_command_adjectiv_id_list(), ","), "==", "a,an,the"),
    }) :));

    expect_function("parse_command_plural_id_list", testOb);
    expect("parse_command_plural_id_list returns list of plurals", (: ({
        assert(implode(testOb->parse_command_plural_id_list(), ","), "==", "things,them,everything"),
    }) :));

    expect_function("parse_command_prepos_list", testOb);
    expect("parse_command_prepos_list returns list of prepositions", (: ({
        assert(sizeof(testOb->parse_command_prepos_list()), "==", 50),
    }) :));

    expect_function("parse_command_all_word", testOb);
    expect("parse_command_all_word returns all word", (: ({
        assert(testOb->parse_command_all_word(), "==", "all"),
    }) :));

    expect_function("parse_command_users", testOb);
    // @TODO test parse_command_users

    expect_function("parser_error_message", testOb);
    // @TODO test parser_error_message

    expect_function("handle_parse_refresh", testOb);
    // @TODO test handle_parse_refresh
}

// UNUSED expect_function("view_errors", testOb);
// UNUSED expect_function("authorfile", testOb);
// UNUSED expect_function("creator_file", testOb);
// UNUSED expect_function("domain_file", testOb);
// UNUSED expect_function("get_root_uid", testOb);