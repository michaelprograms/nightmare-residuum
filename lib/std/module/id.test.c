inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

void test_id () {
    expect_function("query_name", testOb);
    expect_function("query_cap_name", testOb);
    expect_function("query_key_name", testOb);
    expect_function("set_name", testOb);
    expect_function("set_key_name", testOb);
    expect_function("query_id", testOb);
    expect_function("query_key_id", testOb);
    expect_function("set_id", testOb);
    expect_function("remove_id", testOb);
    expect_function("query_adjective", testOb);
    expect_function("query_key_adjective", testOb);
    expect_function("set_adjective", testOb);
    expect_function("add_adjective", testOb);
    expect_function("remove_adjective", testOb);
    expect_function("query_plural", testOb);
    expect_function("add_plural", testOb);
    expect_function("remove_plural", testOb);
    expect_function("parse_command_id_list", testOb);
    expect_function("parse_command_adjectiv_id_list", testOb);
    expect_function("parse_command_plural_id_list", testOb);

    expect("id handles adding names", (: ({
        assert_equal(testOb->query_name(), UNDEFINED),
        assert_equal(testOb->query_cap_name(), UNDEFINED),
        assert_equal(testOb->query_key_name(), UNDEFINED),
        assert_equal(testOb->query_adjective(), ({ })),
        assert_equal(implode(testOb->query_plural(), ","), ""),
        assert_equal(testOb->set_name("Testee"), 0),
        assert_equal(testOb->query_name(), "testee"),
        assert_equal(testOb->query_cap_name(), "Testee"),
        assert_equal(testOb->query_key_name(), "testee"),
    }) :));

    expect("id handles adding id and plural", (: ({
        assert_equal(testOb->set_id(({"testee", "man"})), 0),
        assert_equal(implode(testOb->query_id(), ","), "testee,man"),
        assert_equal(testOb->query_key_id(), "testee"),
        assert_equal(implode(testOb->query_plural(), ","), "testees,men"),
        assert_equal(testOb->add_plural("nads"), 0),
        assert_equal(implode(testOb->query_plural(), ","), "testees,men,nads"),
    }) :));

    expect("id handles adding adjectives", (: ({
        assert_equal(testOb->set_adjective(({ "large" })), 0),
        assert_equal(implode(testOb->query_adjective(), ","), "large"),
        assert_equal(testOb->query_key_adjective(), "large"),
        assert_equal(testOb->add_adjective("big"), 0),
        assert_equal(implode(testOb->query_adjective(), ","), "large,big"),
        assert_equal(testOb->query_key_adjective(), "large"),
    }) :));

    expect("id handles applies", (: ({
        assert_equal(implode(testOb->query_id(), ","), implode(testOb->parse_command_id_list(), ",")),
        assert_equal(implode(testOb->query_adjective(), ","), implode(testOb->parse_command_adjectiv_id_list(), ",")),
        assert_equal(implode(testOb->query_plural(), ","), implode(testOb->parse_command_plural_id_list(), ",")),
    }) :));

    expect("id handles removing id and plural", (: ({
        assert_equal(testOb->remove_plural("nads"), 0),
        assert_equal(implode(testOb->query_plural(), ","), "testees,men"),
        assert_equal(testOb->remove_id(({"man"})), 0),
        assert_equal(implode(testOb->query_id(), ","), "testee"),
        assert_equal(testOb->query_key_id(), "testee"),
        assert_equal(implode(testOb->query_plural(), ","), "testees"),
    }) :));

    expect("id handles removing adjectives", (: ({
        assert_equal(testOb->remove_adjective("large"), 0),
        assert_equal(implode(testOb->query_adjective(), ","), "big"),
        assert_equal(testOb->query_key_adjective(), "big"),
    }) :));
}