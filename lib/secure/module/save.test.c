#define PATH_TEST_SAVE "/save/test/test.o"

inherit M_TEST;

string *test_order () {
    return ({ "test_path", "test_save", "test_restore", });
}

void test_path () {
    expect("paths are settable and queryable", (: ({
        assert_equal(testOb->query_save_path(), 0),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert_equal(testOb->query_save_path(), PATH_TEST_SAVE),

        testOb->set_save_path(0),
        assert_equal(testOb->query_save_path(), 0),
    }) :));
}

void test_save () {
    expect("data can be saved", (: ({
        // no data exists
        assert_equal(file_size(PATH_TEST_SAVE), -1),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert_equal(testOb->query_save_path(), PATH_TEST_SAVE),

        // create dat
        testOb->save_data(),
        assert_equal(file_size(PATH_TEST_SAVE), 53),

        // delete data
        assert_equal(rm(PATH_TEST_SAVE), 1),
    }) :));
}

void test_restore () {
    expect("data can be restored", (: ({
        // no data exists
        assert_equal(file_size(PATH_TEST_SAVE), -1),

        testOb->set_save_path(PATH_TEST_SAVE),
        assert_equal(testOb->query_save_path(), PATH_TEST_SAVE),

        // create save with test data
        assert_equal(write_file(PATH_TEST_SAVE, "#/secure/module/save.coverage.c\n__Created 1234567890\n"), 1),
        assert_equal(file_size(PATH_TEST_SAVE), 53),

        // restore data
        testOb->restore_data(),

        // data was restored
        assert_equal(testOb->query_created(), 1234567890),

        // delete data
        assert_equal(rm(PATH_TEST_SAVE), 1),
    }) :));
}