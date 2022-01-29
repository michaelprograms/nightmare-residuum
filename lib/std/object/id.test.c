inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/object/id");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_id () {
    string *appliesValues = ({}), *appliesResults = ({});
    string *nameValues = ({}), *nameResults = ({});
    string *idValues = ({}), *idResults = ({});
    string *adjValues = ({}), *adjResults = ({});
    string *pluralsValues = ({}), *pluralsResults = ({});

    expect_function("query_name", testOb);
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

    nameValues += ({ testOb->query_name() });
    nameResults += ({ UNDEFINED });
    nameValues += ({ testOb->query_key_name() });
    nameResults += ({ UNDEFINED });
    adjValues += ({ testOb->query_adjective() });
    adjResults += ({ ({}) });
    pluralsValues += ({ identify(testOb->query_plural()) });
    pluralsResults += ({ "({ })" });

    testOb->set_name("Testee");
    nameValues += ({ testOb->query_name() });
    nameResults += ({ "Testee" });
    nameValues += ({ testOb->query_key_name() });
    nameResults += ({ "testee" });

    testOb->set_id(({"testee", "man"}));
    idValues += ({ identify(testOb->query_id()) });
    idResults += ({ identify(({ "testee", "man" })) });
    idValues += ({ testOb->query_key_id() });
    idResults += ({ "testee" });
    pluralsValues += ({ identify(testOb->query_plural()) });
    pluralsResults += ({ identify(({ "testees", "men"})) });

    testOb->add_plural("nads");
    pluralsValues += ({ identify(testOb->query_plural()) });
    pluralsResults += ({ identify(({"testees", "men", "nads"})) });
    testOb->remove_plural("nads");
    pluralsValues += ({ identify(testOb->query_plural()) });
    pluralsResults += ({ identify(({"testees", "men"})) });

    appliesValues += ({ identify(testOb->query_id()) });
    appliesResults += ({ identify(testOb->parse_command_id_list()) });
    appliesValues += ({ identify(testOb->query_adjective()) });
    appliesResults += ({ identify(testOb->parse_command_adjectiv_id_list()) });
    appliesValues += ({ identify(testOb->query_plural()) });
    appliesResults += ({ identify(testOb->parse_command_plural_id_list()) });

    testOb->set_adjective(({ "large" }));
    adjValues += ({ identify(testOb->query_adjective()) });
    adjResults += ({ identify(({ "large" })) });
    adjValues += ({ testOb->query_key_adjective() });
    adjResults += ({ "large" });
    testOb->add_adjective("big");
    adjValues += ({ identify(testOb->query_adjective()) });
    adjResults += ({ identify(({ "large", "big" })) });
    adjValues += ({ testOb->query_key_adjective() });
    adjResults += ({ "large" });
    testOb->remove_adjective("large");
    adjValues += ({ identify(testOb->query_adjective()) });
    adjResults += ({ identify(({ "big" })) });
    adjValues += ({ testOb->query_key_adjective() });
    adjResults += ({ "big" });

    testOb->remove_id(({"man"}));
    idValues += ({ identify(testOb->query_id()) });
    idResults += ({ identify(({ "testee" })) });
    idValues += ({ testOb->query_key_id() });
    idResults += ({ "testee" });
    pluralsValues += ({ identify(testOb->query_plural()) });
    pluralsResults += ({ identify(({ "testees" })) });

    expect_arrays_equal(nameValues, nameResults, "id handled names");
    expect_arrays_equal(idValues, idResults, "id handled ids");
    expect_arrays_equal(adjValues, adjResults, "id handled adjectives");
    expect_arrays_equal(pluralsValues, pluralsResults, "id handled plurals");
    expect_arrays_equal(appliesValues, appliesResults, "id handled applies");
}

void test_id_bad_arguments () {
    expect_catches (({
        (: testOb->set_key_name(0) :),
        (: testOb->set_key_name(0.0) :),
        (: testOb->set_key_name("") :),
        (: testOb->set_key_name(({})) :),
        (: testOb->set_key_name(([])) :),
    }), "*Bad argument 1 to id->set_key_name\n", "set_key_name handled invalid argument 1");
    expect_catches (({
        (: testOb->set_name(0) :),
        (: testOb->set_name(0.0) :),
        (: testOb->set_name("") :),
        (: testOb->set_name(({})) :),
        (: testOb->set_name(([])) :),
    }), "*Bad argument 1 to id->set_name\n", "set_name handled invalid argument 1");
}