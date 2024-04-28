inherit M_TEST;

void test_id () {
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

    expect("id handles bad inputs", (: ({
        assert_catch((: testOb->set_name("") :), "*Bad argument 1 to id->set_name\n"),

        assert_catch((: testOb->set_id(({ })) :), "*Bad argument 1 to id->set_id\n"),
        assert_catch((: testOb->remove_id(({ })) :), "*Bad argument 1 to id->remove_id\n"),

        assert_catch((: testOb->set_adjective(({ })) :), "*Bad argument 1 to id->set_adjective\n"),
        assert_catch((: testOb->add_adjective("") :), "*Bad argument 1 to id->add_adjective\n"),
        assert_catch((: testOb->remove_adjective("") :), "*Bad argument 1 to id->remove_adjective\n"),

        assert_catch((: testOb->set_adjective(({ })) :), "*Bad argument 1 to id->set_adjective\n"),
        assert_catch((: testOb->add_adjective("") :), "*Bad argument 1 to id->add_adjective\n"),
        assert_catch((: testOb->remove_adjective("") :), "*Bad argument 1 to id->remove_adjective\n"),

        assert_catch((: testOb->add_plural("") :), "*Bad argument 1 to id->add_plural\n"),
        assert_catch((: testOb->remove_plural("") :), "*Bad argument 1 to id->remove_plural\n"),
    }) :));

    expect("id handles applies", (: ({
        assert_equal(implode(testOb->query_id(), ","), implode(testOb->parse_command_id_list(), ",")),
        assert_equal(implode(testOb->query_adjective(), ","), implode(testOb->parse_command_adjectiv_id_list(), ",")),
        assert_equal(implode(testOb->query_plural(), ","), implode(testOb->parse_command_plural_id_list(), ",")),
    }) :));
}

void test_key_name () {
    expect("key_name handles setting key_name and id", (: ({
        assert_equal(testOb->query_key_name(), UNDEFINED),
        assert_equal(testOb->query_id(), ({ })),

        testOb->set_key_name("tester"),
        assert_equal(testOb->query_key_name(), "tester"),
        assert_equal(testOb->query_id(), ({ "tester" })),
    }) :));
    expect("key_name handles bad name inputs", (: ({
        assert_catch((: testOb->set_key_name("") :), "*Bad argument 1 to id->set_key_name\n"),
    }) :));
}