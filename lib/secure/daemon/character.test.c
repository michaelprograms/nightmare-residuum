inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/daemon/character");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

// void test_valid_name () {
//     int *values = ({}), *results = ({});

//     expect_function("query_valid_name", testOb);

//     values += ({ testOb->query_valid_name("valid") });
//     results += ({ 1 });

//     values += ({ testOb->query_valid_name("") });
//     results += ({ 0 });

//     expect_arrays_equal (values, results, "query_valid_name handled names");
// }