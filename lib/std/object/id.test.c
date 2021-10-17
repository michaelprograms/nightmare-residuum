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
    string *values = ({}), *results = ({});

    expect_function("query_name", testOb);
    expect_function("query_key_name", testOb);
    expect_function("set_name", testOb);
    expect_function("set_key_name", testOb);
    expect_function("query_id", testOb);
    expect_function("query_key_id", testOb);
    expect_function("set_id", testOb);
    // expect_function("remove_id", testOb);   // @TODO
    expect_function("query_adjective", testOb);
    // expect_function("query_key_adjective", testOb); // @TODO
    // expect_function("set_adjective", testOb);   // @TODO
    // expect_function("add_adjective", testOb);   // @TODO
    // expect_function("remove_adjective", testOb);    // TODO
    expect_function("query_plural", testOb);
    expect_function("add_plural", testOb);
    expect_function("parse_command_id_list", testOb);
    expect_function("parse_command_adjectiv_id_list", testOb);
    expect_function("parse_command_plural_id_list", testOb);

    values += ({ testOb->query_name() });
    results += ({ 0 });
    values += ({ testOb->query_key_name() });
    results += ({ 0 });
    values += ({ testOb->query_adjective() });
    results += ({ ({}) });
    values += ({ testOb->query_plural() });
    results += ({ ({}) });

    testOb->set_name("Testee");
    values += ({ testOb->query_name() });
    results += ({ "Testee" });
    values += ({ testOb->query_key_name() });
    results += ({ "testee" });

    testOb->set_id(({"testee", "man"}));
    values += ({ testOb->query_id() });
    results += ({ "testee", "man" });
    values += ({ testOb->query_key_id() });
    results += ({ "testee" });
    values += ({ testOb->query_plural() });
    results += ({ "testees", "men" });

    testOb->add_plural("nads");
    values += ({ testOb->query_plural() });
    results += ({ "testees", "men", "nads" });

    values += ({ testOb->query_id() });
    results += ({ testOb->parse_command_id_list() });
    values += ({ testOb->query_adjective() });
    results += ({ testOb->parse_command_adjectiv_id_list() });
    values += ({ testOb->query_plural() });
    results += ({ testOb->parse_command_plural_id_list() });

    expect_arrays_array_equal(values, results, "id handled id, name, adjective, and plural");
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