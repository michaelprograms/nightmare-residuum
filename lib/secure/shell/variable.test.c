inherit M_TEST;

private int action = 0;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/shell/variable.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

string *test_order () {
    return ({ "test_variables", "test_variable_hooks" });
}

void test_set_variables () {
    int *values = ({}), *results = ({});

    expect_function("query_variable", testOb);
    expect_function("set_variable", testOb);
    expect_function("unset_variable", testOb);

    values += ({ testOb->query_variable("testkey") });
    results += ({ UNDEFINED });

    values += ({ testOb->set_variable("testkey", 123) });
    results += ({ 123 });
    values += ({ testOb->query_variable("testkey") });
    results += ({ 123 });

    values += ({ testOb->unset_variable("testkey") });
    results += ({ 1 });
    values += ({ testOb->query_variable("testkey") });
    results += ({ UNDEFINED });

    values += ({ testOb->set_variable("testkey", "xyz") });
    results += ({ "xyz" });
    values += ({ testOb->query_variable("testkey") });
    results += ({ "xyz" });

    expect_arrays_equal(values, results, "variables handled");
}

void test_set_variables_if_undefined () {
    int *values = ({}), *results = ({});

    values += ({ testOb->query_variable("testkey") });
    results += ({ UNDEFINED });

    values += ({ testOb->set_variable("testkey", 123, 1) });
    results += ({ 123 });
    values += ({ testOb->query_variable("testkey") });
    results += ({ 123 });

    values += ({ testOb->set_variable("testkey", 987, 1) });
    results += ({ 123 }); // didn't set

    values += ({ testOb->unset_variable("testkey") });
    results += ({ 1 });
    values += ({ testOb->query_variable("testkey") });
    results += ({ UNDEFINED });

    values += ({ testOb->set_variable("testkey", 987, 1) });
    results += ({ 987 }); // did set

    expect_arrays_equal(values, results, "variable_if_undefined handled");
}

void test_set_variable_hooks () {
    int *values = ({}), *results = ({});

    expect_function("set_variable_hook", testOb);

    testOb->set_variable_hook("testkey", function (mixed value) {
        action ++;
        return value;
    });

    values += ({ action });
    results += ({ 0 });

    values += ({ testOb->set_variable("testkey", 123) });
    results += ({ 123 });
    values += ({ action });
    results += ({ 1 });

    values += ({ testOb->set_variable("testkey", "xyz") });
    results += ({ "xyz" });
    values += ({ action });
    results += ({ 2 });

    expect_arrays_equal(values, results, "hooks handled");
}