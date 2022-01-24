inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living/vitals.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_update_vitals", "test_hp", "test_sp", "test_mp" });
}

void test_update_vitals () {
    int *values = ({}), *results = ({});

    expect_function("update_vitals", testOb);

    // initial values
    values += ({ testOb->query_hp() });
    results += ({ 0 });
    values += ({ testOb->query_max_hp() });
    results += ({ 0 });
    values += ({ testOb->query_sp() });
    results += ({ 0 });
    values += ({ testOb->query_max_sp() });
    results += ({ 0 });
    values += ({ testOb->query_mp() });
    results += ({ 0 });
    values += ({ testOb->query_max_mp() });
    results += ({ 0 });

    testOb->update_vitals(0);   // don't heal
    values += ({ testOb->query_hp() });
    results += ({ 0 });
    values += ({ testOb->query_max_hp() });
    results += ({ 20 });
    values += ({ testOb->query_sp() });
    results += ({ 0 });
    values += ({ testOb->query_max_sp() });
    results += ({ 10 });
    values += ({ testOb->query_mp() });
    results += ({ 0 });
    values += ({ testOb->query_max_mp() });
    results += ({ 10 });

    testOb->update_vitals(1);   // heal
    values += ({ testOb->query_hp() });
    results += ({ 20 });
    values += ({ testOb->query_hp() == testOb->query_max_hp() });
    results += ({ 1 });
    values += ({ testOb->query_sp() });
    results += ({ 10 });
    values += ({ testOb->query_sp() == testOb->query_max_sp() });
    results += ({ 1 });
    values += ({ testOb->query_mp() });
    results += ({ 10 });
    values += ({ testOb->query_mp() == testOb->query_max_mp() });
    results += ({ 1 });

    expect_arrays_equal(values, results, "update_vitals handled base and max hp/sp/mp");
}

void test_hp () {
    int *values = ({}), *results = ({});

    expect_function("set_hp", testOb);
    expect_function("add_hp", testOb);
    expect_function("query_hp", testOb);
    expect_function("query_max_hp", testOb);

    testOb->add_hp(1);  // already max
    values += ({ testOb->query_hp() });
    results += ({ 20 });

    testOb->add_hp(-10);
    values += ({ testOb->query_hp() });
    results += ({ 10 });
    testOb->add_hp(5);
    values += ({ testOb->query_hp() });
    results += ({ 15 });
    testOb->add_hp(0);
    values += ({ testOb->query_hp() });
    results += ({ 15 });
    testOb->add_hp(10); // attempt to go over max
    values += ({ testOb->query_hp() });
    results += ({ 20 });

    testOb->set_hp(5);
    values += ({ testOb->query_hp() });
    results += ({ 5 });

    testOb->set_hp(0);
    values += ({ testOb->query_hp() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "hp handled set/query/query_max");
}

void test_sp () {
    int *values = ({}), *results = ({});

    expect_function("set_sp", testOb);
    expect_function("add_sp", testOb);
    expect_function("query_sp", testOb);
    expect_function("query_max_sp", testOb);

    testOb->add_sp(1);  // already max
    values += ({ testOb->query_sp() });
    results += ({ 10 });

    testOb->add_sp(-5);
    values += ({ testOb->query_sp() });
    results += ({ 5 });
    testOb->add_sp(2);
    values += ({ testOb->query_sp() });
    results += ({ 7 });
    testOb->add_sp(0);
    values += ({ testOb->query_sp() });
    results += ({ 7 });
    testOb->add_sp(5); // attespt to go over max
    values += ({ testOb->query_sp() });
    results += ({ 10 });

    testOb->set_sp(5);
    values += ({ testOb->query_sp() });
    results += ({ 5 });

    testOb->set_sp(0);
    values += ({ testOb->query_sp() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "sp handled set/query/query_max");
}

void test_mp () {
    int *values = ({}), *results = ({});

    expect_function("set_mp", testOb);
    expect_function("add_mp", testOb);
    expect_function("query_mp", testOb);
    expect_function("query_max_mp", testOb);

    testOb->add_mp(1);  // already max
    values += ({ testOb->query_mp() });
    results += ({ 10 });

    testOb->add_mp(-5);
    values += ({ testOb->query_mp() });
    results += ({ 5 });
    testOb->add_mp(2);
    values += ({ testOb->query_mp() });
    results += ({ 7 });
    testOb->add_mp(0);
    values += ({ testOb->query_mp() });
    results += ({ 7 });
    testOb->add_mp(5); // attempt to go over max
    values += ({ testOb->query_mp() });
    results += ({ 10 });

    testOb->set_mp(5);
    values += ({ testOb->query_mp() });
    results += ({ 5 });

    testOb->set_mp(0);
    values += ({ testOb->query_mp() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "mp handled set/query/query_max");
}