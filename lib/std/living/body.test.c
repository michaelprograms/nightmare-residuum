inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living/body.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_gender () {
    string *values = ({}), *results = ({});

    expect_function("set_gender", testOb);
    expect_function("query_gender", testOb);

    values += ({ testOb->query_gender() });
    results += ({ "neither" });
    testOb->set_gender("male");
    values += ({ testOb->query_gender() });
    results += ({ "male" });
    testOb->set_gender("female");
    values += ({ testOb->query_gender() });
    results += ({ "female" });
    testOb->set_gender("neither");
    values += ({ testOb->query_gender() });
    results += ({ "neither" });
    testOb->set_gender(0);
    values += ({ testOb->query_gender() });
    results += ({ "none" });
    testOb->set_gender("invalid");
    values += ({ testOb->query_gender() });
    results += ({ "none" });

    expect_arrays_equal(values, results, "gender handled");
}

void test_level () {
    int *values = ({}), *results = ({});

    expect_function("set_level", testOb);
    expect_function("query_level", testOb);

    values += ({ testOb->query_level() });
    results += ({ 0 });

    testOb->set_level(123);
    values += ({ testOb->query_level() });
    results += ({ 123 });

    testOb->set_level(0);
    values += ({ testOb->query_level() });
    results += ({ 0 });

    testOb->set_level(-123);
    values += ({ testOb->query_level() });
    results += ({ 123 });

    expect_arrays_equal(values, results, "handled level");
}

void test_experience () {
    int *values = ({}), *results = ({});

    expect_function("add_experience", testOb);
    expect_function("spend_experience", testOb);
    expect_function("query_experience", testOb);
    expect_function("query_total_experience", testOb);

    values += ({ testOb->query_experience() });
    results += ({ 0 });
    values += ({ testOb->query_total_experience() });
    results += ({ 0 });

    testOb->add_experience(123);
    values += ({ testOb->query_experience() });
    results += ({ 123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 0 });

    testOb->add_experience(9000);
    values += ({ testOb->query_experience() });
    results += ({ 9123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 0 });

    testOb->spend_experience(1000);
    values += ({ testOb->query_experience() });
    results += ({ 8123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 1000 });

    testOb->spend_experience(2000);
    values += ({ testOb->query_experience() });
    results += ({ 6123 });
    values += ({ testOb->query_total_experience() });
    results += ({ 3000 });

    expect_arrays_equal(values, results, "handled experience");

    expect_catches (({
        (: testOb->spend_experience(10000) :),
        (: testOb->spend_experience(0) :),
        (: testOb->spend_experience(-10000) :),
    }), "*Bad argument 1 to body->spend_experience\n", "spend_experience handled bad inputs ");
}

void test_victory_and_defeats () {
    int *values = ({}), *results = ({});

    expect_function("add_victory", testOb);
    expect_function("query_victory", testOb);
    expect_function("add_defeat", testOb);
    expect_function("query_defeat", testOb);

    values += ({ testOb->query_victory() });
    results += ({ 0 });
    values += ({ testOb->query_defeat() });
    results += ({ 0 });

    testOb->add_victory();
    testOb->add_defeat();
    values += ({ testOb->query_victory() });
    results += ({ 1 });
    values += ({ testOb->query_defeat() });
    results += ({ 1 });

    testOb->add_victory();
    testOb->add_defeat();
    values += ({ testOb->query_victory() });
    results += ({ 2 });
    values += ({ testOb->query_defeat() });
    results += ({ 2 });

    expect_arrays_equal(values, results, "handled victory and defeat counters");
}