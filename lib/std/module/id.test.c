inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/id.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
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
        assert(testOb->query_name(), "==", UNDEFINED),
        assert(testOb->query_cap_name(), "==", UNDEFINED),
        assert(testOb->query_key_name(), "==", UNDEFINED),
        assert(testOb->query_adjective(), "==", ({ })),
        assert(implode(testOb->query_plural(), ","), "==", ""),
        assert(testOb->set_name("Testee"), "==", 0),
        assert(testOb->query_name(), "==", "testee"),
        assert(testOb->query_cap_name(), "==", "Testee"),
        assert(testOb->query_key_name(), "==", "testee"),
    }) :));

    expect("id handles adding id and plural", (: ({
        assert(testOb->set_id(({"testee", "man"})), "==", 0),
        assert(implode(testOb->query_id(), ","), "==", "testee,man"),
        assert(testOb->query_key_id(), "==", "testee"),
        assert(implode(testOb->query_plural(), ","), "==", "testees,men"),
        assert(testOb->add_plural("nads"), "==", 0),
        assert(implode(testOb->query_plural(), ","), "==", "testees,men,nads"),
    }) :));

    expect("id handles adding adjectives", (: ({
        assert(testOb->set_adjective(({ "large" })), "==", 0),
        assert(implode(testOb->query_adjective(), ","), "==", "large"),
        assert(testOb->query_key_adjective(), "==", "large"),
        assert(testOb->add_adjective("big"), "==", 0),
        assert(implode(testOb->query_adjective(), ","), "==", "large,big"),
        assert(testOb->query_key_adjective(), "==", "large"),
    }) :));

    expect("id handles applies", (: ({
        assert(implode(testOb->query_id(), ","), "==", implode(testOb->parse_command_id_list(), ",")),
        assert(implode(testOb->query_adjective(), ","), "==", implode(testOb->parse_command_adjectiv_id_list(), ",")),
        assert(implode(testOb->query_plural(), ","), "==", implode(testOb->parse_command_plural_id_list(), ",")),
    }) :));

    expect("id handles removing id and plural", (: ({
        assert(testOb->remove_plural("nads"), "==", 0),
        assert(implode(testOb->query_plural(), ","), "==", "testees,men"),
        assert(testOb->remove_id(({"man"})), "==", 0),
        assert(implode(testOb->query_id(), ","), "==", "testee"),
        assert(testOb->query_key_id(), "==", "testee"),
        assert(implode(testOb->query_plural(), ","), "==", "testees"),
    }) :));

    expect("id handles removing adjectives", (: ({
        assert(testOb->remove_adjective("large"), "==", 0),
        assert(implode(testOb->query_adjective(), ","), "==", "big"),
        assert(testOb->query_key_adjective(), "==", "big"),
    }) :));
}