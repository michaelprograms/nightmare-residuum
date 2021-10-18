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

void test_applies () {
    object ob;

    expect_function("connect", testOb);

    expect_strings_equal(base_name(ob = testOb->connect()), OBJ_USER, "connect returned OBJ_USER");

    destruct(ob);
}

void test_startup_applies () {
    expect_function("epilog", testOb);
    expect_true(typeof(testOb->epilog()) == "array", "epilog returned preload array");

    expect_function("flag", testOb);

    expect_function("preload", testOb);
}

void test_build_applies () {
    expect_function("compile_object", testOb);
    // @TODO test compile_object here

    expect_function("object_name", testOb);

    expect_function("get_include_path", testOb);
    expect_arrays_equal(testOb->get_include_path("/test/Test"), ({ ":DEFAULT:" }), "get_include_path handled /test/Test");
    expect_arrays_array_equal(({
        testOb->get_include_path("/domain/Name"),
        testOb->get_include_path("/domain/Name/test"),
        testOb->get_include_path("/domain/Name/test/dir"),
    }), ({ ":DEFAULT:", "/domain/Name/include" }), "get_include_path handled /domain/Name");
    expect_arrays_array_equal(({
        testOb->get_include_path("/realm/Name"),
        testOb->get_include_path("/realm/Name/test"),
        testOb->get_include_path("/realm/Name/test/dir"),
    }), ({ ":DEFAULT:", "/realm/Name/include" }), "get_include_path handled /realm/Name");
}

void test_error_applies () {
    expect_function("crash", testOb);
    expect_function("error_handler", testOb);
    expect_function("log_error", testOb);
}

// void test_ed_applies () {
//     expect_function("get_save_file_name", testOb);

//     expect_function("make_path_absolute", testOb);
//     // make_absolute_path testing handled by sefun sanitize_path

//     expect_function("retrieve_ed_setup", testOb);

//     expect_function("save_ed_setup", testOb);
// }

void test_security_applies () {
    int *values = ({}), *results = ({});

    expect_function("privs_file", testOb);
    // privs_file testing handled by D_ACCESS query_file_privs

    expect_function("valid_override", testOb);
    values += ({ testOb->valid_override("/secure/sefun/override") });
    results += ({ 1 });

    values += ({ testOb->valid_override("/insecure") });
    results += ({ 0 });

    values += ({ testOb->valid_override("/secure/user/input", "input_to") });
    results += ({ 1 });

    values += ({ testOb->valid_override("/secure/user/input", "get_char") });
    results += ({ 1 });

    expect_arrays_equal(values, results, "valid_override handled requests");
}

void test_valid_applies () {
    object basicOb;

    // expect_function("valid_bind", testOb);
    // expect_function("valid_database", testOb);
    // expect_function("valid_hide", testOb);
    // expect_function("valid_link", testOb);
    // expect_function("valid_object", testOb);

    expect_function("valid_read", testOb);
    expect_true(testOb->valid_read("/", testOb, "read_file"), "valid_read handled allowed call");
    // @TODO find something that doesn't have valid read

    expect_function("valid_write", testOb);
    expect_true(testOb->valid_write("/", testOb, "write_file"), "valid_write handled allowed call");
    expect_false(testOb->valid_write("/save", (basicOb = new (STD_OBJECT)), "write_file"), "valid_write handled denied call");
    destruct(basicOb);

    // expect_function("valid_save_binary", testOb);
    // expect_function("valid_seteuid", testOb);
    // expect_function("valid_shadow", testOb);
    // expect_function("valid_socket", testOb);
}

void test_parsing_applies () {
    expect_function("parse_command_id_list", testOb);
    expect_strings_equal(implode(testOb->parse_command_id_list(), ","), "thing", "parse_command_id_list returns list of nouns");

    expect_function("parse_command_adjectiv_id_list", testOb);
    expect_strings_equal(implode(testOb->parse_command_adjectiv_id_list(), ","), "a,an,the", "parse_command_adjectiv_id_list returns list of adjectives");

    expect_function("parse_command_plural_id_list", testOb);
    expect_strings_equal(implode(testOb->parse_command_plural_id_list(), ","),  "things,them,everything", "parse_command_plural_id_list returns list of plurals");

    expect_function("parse_command_prepos_list", testOb);
    expect_integers_equal(sizeof(testOb->parse_command_prepos_list()), 50, "parse_command_prepos_list returns list of prepositions");

    expect_function("parse_command_all_word", testOb);
    expect_strings_equal(testOb->parse_command_all_word(), "all", "parse_command_all_word returns word");

    expect_function("parse_command_users", testOb);
    // @TODO test parse_command_users

    expect_function("parser_error_message", testOb);
    // @TODO test parser_error_message

    expect_function("handle_parse_refresh", testOb);
    // @TODO test handle_parse_refresh
}
// TODO? expect_function("get_mud_stats", testOb);

// UNUSED expect_function("view_errors", testOb);
// UNUSED expect_function("authorfile", testOb);
// UNUSED expect_function("creator_file", testOb);
// UNUSED expect_function("domain_file", testOb);
// UNUSED expect_function("get_root_uid", testOb);