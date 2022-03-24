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
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
    unguarded((: rm, PATH_TEST_SAVE :));
}

string *test_order () {
    return ({ "test_save_path", "test_save_data", });
}

void test_save_path () {
    expect_function("query_save_path", testOb);
    expect_function("set_save_path", testOb);

    expect("save handles paths", (: ({
        unguarded((: rm, PATH_TEST_SAVE :)),
        assert(testOb->query_save_path(), "==", 0),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert(testOb->query_save_path(), "==", PATH_TEST_SAVE),

        testOb->set_save_path(0),
        assert(testOb->query_save_path(), "==", 0),

    }) :));
}

void test_save_data () {
    expect_function("restore_data", testOb);
    expect_function("save_data", testOb);

    expect("save handles data", (: ({
        unguarded((: rm, PATH_TEST_SAVE :)),
        assert(file_size(PATH_TEST_SAVE), "==", -1),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert(testOb->query_save_path(), "==", PATH_TEST_SAVE),

        unguarded((: call_other, testOb, "save_data" :)),
        assert(file_size(PATH_TEST_SAVE), ">", 0),
    }) :));
}