#define PATH_TEST_SAVE "/save/test/test.o"

inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    unguarded((: assure_dir, PATH_TEST_SAVE :));
    unguarded((: rm, PATH_TEST_SAVE :));
}
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/secure/module/save.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
    unguarded((: rm, PATH_TEST_SAVE :));
}

string *test_order () {
    return ({ "test_save_path", "test_save_data", });
}

void test_save_path () {
    string *values = ({}), *results = ({});

    expect_function("query_save_path", testOb);
    expect_function("set_save_path", testOb);

    unguarded((: rm, PATH_TEST_SAVE :));
    values += ({ testOb->query_save_path() });
    results += ({ 0 });

    testOb->set_save_path(PATH_TEST_SAVE);
    values += ({ testOb->query_save_path() });
    results += ({ PATH_TEST_SAVE });

    testOb->set_save_path(0);
    values += ({ testOb->query_save_path() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "save handled path");
}

void test_save_data () {
    string *values = ({}), *results = ({});

    expect_function("restore_data", testOb);
    expect_function("save_data", testOb);

    unguarded((: rm, PATH_TEST_SAVE :));
    values += ({ file_size(PATH_TEST_SAVE) });
    results += ({ -1 });

    testOb->set_save_path(PATH_TEST_SAVE);
    values += ({ testOb->query_save_path() });
    results += ({ PATH_TEST_SAVE });

    unguarded((: call_other, testOb, "save_data" :));
    values += ({ (file_size(PATH_TEST_SAVE) > 0) });
    results += ({ 1 });

    expect_arrays_equal(values, results, "save handled data");
}