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
        assert(testOb->query_adjective(), "==", ({})),
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

void test_id_bad_arguments () {
    expect("set_key_name handled invalid argument 1", (: ({
        assert((: testOb->set_key_name(0) :), "catch", "*Bad argument 1 to id->set_key_name\n"),
        assert((: testOb->set_key_name(0.0) :), "catch", "*Bad argument 1 to id->set_key_name\n"),
        assert((: testOb->set_key_name("") :), "catch", "*Bad argument 1 to id->set_key_name\n"),
        assert((: testOb->set_key_name(({})) :), "catch", "*Bad argument 1 to id->set_key_name\n"),
        assert((: testOb->set_key_name(([])) :), "catch", "*Bad argument 1 to id->set_key_name\n"),
    }) :));
    expect("set_name handled invalid argument 1", (: ({
        assert((: testOb->set_name(0) :), "catch", "*Bad argument 1 to id->set_name\n"),
        assert((: testOb->set_name(0.0) :), "catch", "*Bad argument 1 to id->set_name\n"),
        assert((: testOb->set_name("") :), "catch", "*Bad argument 1 to id->set_name\n"),
        assert((: testOb->set_name(({})) :), "catch", "*Bad argument 1 to id->set_name\n"),
        assert((: testOb->set_name(([])) :), "catch", "*Bad argument 1 to id->set_name\n"),
    }) :));
    expect("set_id handled invalid argument 1", (: ({
        assert((: testOb->set_id(0) :), "catch", "*Bad argument 1 to id->set_id\n"),
        assert((: testOb->set_id(0.0) :), "catch", "*Bad argument 1 to id->set_id\n"),
        assert((: testOb->set_id("") :), "catch", "*Bad argument 1 to id->set_id\n"),
        assert((: testOb->set_id(({})) :), "catch", "*Bad argument 1 to id->set_id\n"),
        assert((: testOb->set_id(([])) :), "catch", "*Bad argument 1 to id->set_id\n"),
    }) :));
    expect("remove_id handled invalid argument 1", (: ({
        assert((: testOb->remove_id(0) :), "catch", "*Bad argument 1 to id->remove_id\n"),
        assert((: testOb->remove_id(0.0) :), "catch", "*Bad argument 1 to id->remove_id\n"),
        assert((: testOb->remove_id("") :), "catch", "*Bad argument 1 to id->remove_id\n"),
        assert((: testOb->remove_id(({})) :), "catch", "*Bad argument 1 to id->remove_id\n"),
        assert((: testOb->remove_id(([])) :), "catch", "*Bad argument 1 to id->remove_id\n"),
    }) :));
    expect("set_adjective handled invalid argument 1", (: ({
        assert((: testOb->set_adjective(0) :), "catch", "*Bad argument 1 to id->set_adjective\n"),
        assert((: testOb->set_adjective(0.0) :), "catch", "*Bad argument 1 to id->set_adjective\n"),
        assert((: testOb->set_adjective("") :), "catch", "*Bad argument 1 to id->set_adjective\n"),
        assert((: testOb->set_adjective(({})) :), "catch", "*Bad argument 1 to id->set_adjective\n"),
        assert((: testOb->set_adjective(([])) :), "catch", "*Bad argument 1 to id->set_adjective\n"),
    }) :));
    expect("add_adjective handled invalid argument 1", (: ({
        assert((: testOb->add_adjective(0) :), "catch", "*Bad argument 1 to id->add_adjective\n"),
        assert((: testOb->add_adjective(0.0) :), "catch", "*Bad argument 1 to id->add_adjective\n"),
        assert((: testOb->add_adjective("") :), "catch", "*Bad argument 1 to id->add_adjective\n"),
        assert((: testOb->add_adjective(({})) :), "catch", "*Bad argument 1 to id->add_adjective\n"),
        assert((: testOb->add_adjective(([])) :), "catch", "*Bad argument 1 to id->add_adjective\n"),
    }) :));
    expect("remove_adjective handled invalid argument 1", (: ({
        assert((: testOb->remove_adjective(0) :), "catch", "*Bad argument 1 to id->remove_adjective\n"),
        assert((: testOb->remove_adjective(0.0) :), "catch", "*Bad argument 1 to id->remove_adjective\n"),
        assert((: testOb->remove_adjective("") :), "catch", "*Bad argument 1 to id->remove_adjective\n"),
        assert((: testOb->remove_adjective(({})) :), "catch", "*Bad argument 1 to id->remove_adjective\n"),
        assert((: testOb->remove_adjective(([])) :), "catch", "*Bad argument 1 to id->remove_adjective\n"),
    }) :));
    expect("add_plural handled invalid argument 1", (: ({
        assert((: testOb->add_plural(0) :), "catch", "*Bad argument 1 to id->add_plural\n"),
        assert((: testOb->add_plural(0.0) :), "catch", "*Bad argument 1 to id->add_plural\n"),
        assert((: testOb->add_plural("") :), "catch", "*Bad argument 1 to id->add_plural\n"),
        assert((: testOb->add_plural(({})) :), "catch", "*Bad argument 1 to id->add_plural\n"),
        assert((: testOb->add_plural(([])) :), "catch", "*Bad argument 1 to id->add_plural\n"),
    }) :));
    expect("remove_plural handled invalid argument 1", (: ({
        assert((: testOb->remove_plural(0) :), "catch", "*Bad argument 1 to id->remove_plural\n"),
        assert((: testOb->remove_plural(0.0) :), "catch", "*Bad argument 1 to id->remove_plural\n"),
        assert((: testOb->remove_plural("") :), "catch", "*Bad argument 1 to id->remove_plural\n"),
        assert((: testOb->remove_plural(({})) :), "catch", "*Bad argument 1 to id->remove_plural\n"),
        assert((: testOb->remove_plural(([])) :), "catch", "*Bad argument 1 to id->remove_plural\n"),
    }) :));
}