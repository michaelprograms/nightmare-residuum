#define PATH_TEST_SAVE "/save/test/test.o"

inherit M_TEST;

string *test_order () {
    return ({ "test_save_path", "test_save_data", });
}

void test_save_path () {
    expect("save handles paths", (: ({
        unguarded((: rm, PATH_TEST_SAVE :)),
        assert_equal(testOb->query_save_path(), 0),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert_equal(testOb->query_save_path(), PATH_TEST_SAVE),

        testOb->set_save_path(0),
        assert_equal(testOb->query_save_path(), 0),

    }) :));
}

void test_save_data () {
    expect("save handles data", (: ({
        unguarded((: rm, PATH_TEST_SAVE :)),
        assert_equal(file_size(PATH_TEST_SAVE), -1),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert_equal(testOb->query_save_path(), PATH_TEST_SAVE),

        unguarded((: call_other, testOb, "save_data" :)),
        assert_equal(file_size(PATH_TEST_SAVE) > 0, 1),
    }) :));
}