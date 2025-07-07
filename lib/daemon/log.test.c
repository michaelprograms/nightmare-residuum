inherit M_TEST;

/**
 * @var {"/daemon/log"} testOb
 */

string time;
void test_log () {
    expect("log should handle invalid input", (: ({
        assert_equal(testOb->log("", ""), 0),
        assert_equal(testOb->log("test", ""), 0),
    }):));

    expect("log should log", (: ({
        assert_equal(file_size("/log/test"), -1),
        assert_equal(testOb->log("test", "test1\n"), 1),
        assert_equal(testOb->log("test", "test2"), 1),
        assert_equal(read_file("/log/test"), "test1\ntest2\n"),
        // clean file
        assert_equal(rm("/log/test"), 1),
    }):));

    expect("log should roll", (: ({
        // queue up log roll
        assert_equal(write_file("/log/test", sprintf("%20000s", ".")), 1),
        assert_equal(file_size("/log/test"), 20000),
        assert_equal(sizeof(get_dir("/log/test-*")), 0),
        // log roll
        assert_equal(testOb->log("test", "."), 1),
        assert_equal(sizeof(get_dir("/log/test-*")), 1),

        time = explode(get_dir("/log/test-*")[0], "-")[1],
        assert_equal(file_size("/log/test-"+time), 20000),
        assert_equal(file_size("/log/test"), 2),
        // clean files
        assert_equal(rm("/log/test"), 1),
        assert_equal(rm("/log/test-"+time), 1),
    }) :));
}

void test_log_unique () {
    expect("log_unique should handle invalid input", (: ({
        assert_equal(testOb->log_unique("", ""), 0),
        assert_equal(testOb->log_unique("test", ""), 0),
    }):));

    expect("log_unique should log", (: ({
        assert_equal(file_size("/log/unique/test"), -1),
        assert_equal(testOb->log_unique("test", "test1\n"), 1),
        assert_equal(testOb->log_unique("test", "test1"), 0), // already added
        assert_equal(testOb->log_unique("test", "test2"), 1),
        assert_equal(testOb->log_unique("test", "test2"), 0), // already added
        assert_equal(read_file("/log/unique/test"), "test1\ntest2\n"),
        // clean file
        assert_equal(rm("/log/unique/test"), 1),
    }):));
}