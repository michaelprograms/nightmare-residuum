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
    pluralsValues += ({ implode(testOb->query_plural(), ",") });
    pluralsResults += ({ "" });

    testOb->set_name("Testee");
    nameValues += ({ testOb->query_name() });
    nameResults += ({ "Testee" });
    nameValues += ({ testOb->query_key_name() });
    nameResults += ({ "testee" });

    testOb->set_id(({"testee", "man"}));
    idValues += ({ implode(testOb->query_id(), ",") });
    idResults += ({ "testee,man" });
    idValues += ({ testOb->query_key_id() });
    idResults += ({ "testee" });
    pluralsValues += ({ implode(testOb->query_plural(), ",") });
    pluralsResults += ({ "testees,men" });

    testOb->add_plural("nads");
    pluralsValues += ({ implode(testOb->query_plural(), ",") });
    pluralsResults += ({ "testees,men,nads" });
    testOb->remove_plural("nads");
    pluralsValues += ({ implode(testOb->query_plural(), ",") });
    pluralsResults += ({ "testees,men" });

    appliesValues += ({ implode(testOb->query_id(), ",") });
    appliesResults += ({ implode(testOb->parse_command_id_list(), ",") });
    appliesValues += ({ implode(testOb->query_adjective(), ",") });
    appliesResults += ({ implode(testOb->parse_command_adjectiv_id_list(), ",") });
    appliesValues += ({ implode(testOb->query_plural(), ",") });
    appliesResults += ({ implode(testOb->parse_command_plural_id_list(), ",") });

    testOb->set_adjective(({ "large" }));
    adjValues += ({ implode(testOb->query_adjective(), ",") });
    adjResults += ({ "large" });
    adjValues += ({ testOb->query_key_adjective() });
    adjResults += ({ "large" });
    testOb->add_adjective("big");
    adjValues += ({ implode(testOb->query_adjective(), ",") });
    adjResults += ({ "large,big" });
    adjValues += ({ testOb->query_key_adjective() });
    adjResults += ({ "large" });
    testOb->remove_adjective("large");
    adjValues += ({ implode(testOb->query_adjective(), ",") });
    adjResults += ({ "big" });
    adjValues += ({ testOb->query_key_adjective() });
    adjResults += ({ "big" });

    testOb->remove_id(({"man"}));
    idValues += ({ implode(testOb->query_id(), ",") });
    idResults += ({ "testee" });
    idValues += ({ testOb->query_key_id() });
    idResults += ({ "testee" });
    pluralsValues += ({ implode(testOb->query_plural(), ",") });
    pluralsResults += ({ "testees" });

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
    expect_catches (({
        (: testOb->set_id(0) :),
        (: testOb->set_id(0.0) :),
        (: testOb->set_id("") :),
        (: testOb->set_id(({})) :),
        (: testOb->set_id(([])) :),
    }), "*Bad argument 1 to id->set_id\n", "set_id handled invalid argument 1");
    expect_catches (({
        (: testOb->remove_id(0) :),
        (: testOb->remove_id(0.0) :),
        (: testOb->remove_id("") :),
        (: testOb->remove_id(({})) :),
        (: testOb->remove_id(([])) :),
    }), "*Bad argument 1 to id->remove_id\n", "remove_id handled invalid argument 1");
    expect_catches (({
        (: testOb->set_adjective(0) :),
        (: testOb->set_adjective(0.0) :),
        (: testOb->set_adjective("") :),
        (: testOb->set_adjective(({})) :),
        (: testOb->set_adjective(([])) :),
    }), "*Bad argument 1 to id->set_adjective\n", "set_adjective handled invalid argument 1");
    expect_catches (({
        (: testOb->add_adjective(0) :),
        (: testOb->add_adjective(0.0) :),
        (: testOb->add_adjective("") :),
        (: testOb->add_adjective(({})) :),
        (: testOb->add_adjective(([])) :),
    }), "*Bad argument 1 to id->add_adjective\n", "add_adjective handled invalid argument 1");
    expect_catches (({
        (: testOb->remove_adjective(0) :),
        (: testOb->remove_adjective(0.0) :),
        (: testOb->remove_adjective("") :),
        (: testOb->remove_adjective(({})) :),
        (: testOb->remove_adjective(([])) :),
    }), "*Bad argument 1 to id->remove_adjective\n", "remove_adjective handled invalid argument 1");
    expect_catches (({
        (: testOb->add_plural(0) :),
        (: testOb->add_plural(0.0) :),
        (: testOb->add_plural("") :),
        (: testOb->add_plural(({})) :),
        (: testOb->add_plural(([])) :),
    }), "*Bad argument 1 to id->add_plural\n", "add_plural handled invalid argument 1");
    expect_catches (({
        (: testOb->remove_plural(0) :),
        (: testOb->remove_plural(0.0) :),
        (: testOb->remove_plural("") :),
        (: testOb->remove_plural(({})) :),
        (: testOb->remove_plural(([])) :),
    }), "*Bad argument 1 to id->remove_plural\n", "remove_plural handled invalid argument 1");
}