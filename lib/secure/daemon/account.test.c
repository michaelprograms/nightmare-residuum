inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/daemon/account");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_valid_name () {
    int *values = ({}), *results = ({});

    expect_function("query_valid_name", testOb);

    values += ({ testOb->query_valid_name("valid") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("VALID") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("V4L1D") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("Valid123") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("valid_") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("valid@") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("valid.") });
    results += ({ 1 });
    values += ({ testOb->query_valid_name("valid-") });
    results += ({ 1 });

    values += ({ testOb->query_valid_name("") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name("no") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name("nonononononononononononononononononononononononononononononononom") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name("invalid!@#") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name(",<.>;:'\"[{]}\\|=+!#$%^&*()~`") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name("_valid") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name("@valid") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name(".valid") });
    results += ({ 0 });
    values += ({ testOb->query_valid_name("-valid") });
    results += ({ 0 });

    expect_arrays_equal (values, results, "query_valid_name handled names");
}

void test_valid_account () {
    int *values = ({}), *results = ({});

    expect_function("query_exists", testOb);

    values += ({ testOb->query_exists("testaccount") });
    results += ({ 1 });

    values += ({ testOb->query_exists("") });
    results += ({ 0 });
    values += ({ testOb->query_exists("testaccountinvalid") });
    results += ({ 0 });

    expect_arrays_equal (values, results, "query_exists handled accounts");
}