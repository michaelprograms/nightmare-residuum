inherit M_TEST;

/**
 * @var {"/secure/daemon/account"} testOb
 */

void test_valid_name () {
    expect("query_valid_name handles invalid names", (: ({
        assert_equal(testOb->query_valid_name(""), 0),
        assert_equal(testOb->query_valid_name("no"), 0),
        assert_equal(testOb->query_valid_name("nonononononononononononononononononononononononononononononononom"), 0),
        assert_equal(testOb->query_valid_name("invalid!@#"), 0),
        assert_equal(testOb->query_valid_name("INVALID!@#"), 0),
        assert_equal(testOb->query_valid_name(",<.>;:'\"[{]}\\|=+!#$%^&*()~`"), 0),
        assert_equal(testOb->query_valid_name("_valid"), 0),
        assert_equal(testOb->query_valid_name("@valid"), 0),
        assert_equal(testOb->query_valid_name(".valid"), 0),
        assert_equal(testOb->query_valid_name("-valid"), 0),
        assert_equal(testOb->query_valid_name("_VALID"), 0),
        assert_equal(testOb->query_valid_name("@VALID"), 0),
        assert_equal(testOb->query_valid_name(".VALID"), 0),
        assert_equal(testOb->query_valid_name("-VALID"), 0),
    }) :));
    expect("query_valid_name handles valid names", (: ({
        assert_equal(testOb->query_valid_name("valid"), 1),
        assert_equal(testOb->query_valid_name("VALID"), 1),
        assert_equal(testOb->query_valid_name("V4L1D"), 1),
        assert_equal(testOb->query_valid_name("Valid123"), 1),
        assert_equal(testOb->query_valid_name("valid_"), 1),
        assert_equal(testOb->query_valid_name("valid@"), 1),
        assert_equal(testOb->query_valid_name("valid."), 1),
        assert_equal(testOb->query_valid_name("valid-"), 1),
    }) :));
}

void test_save_path () {
    expect("query_save_path handles invalid names", (: ({
        assert_equal(testOb->query_save_path(""), 0),
        assert_equal(testOb->query_save_path("no"), 0),
        assert_equal(testOb->query_save_path("nonononononononononononononononononononononononononononononononom"), 0),
    }) :));
    expect("query_save_path handles valid names", (: ({
        assert_regex(testOb->query_save_path("name"), "^/save/account/n/name"),
        assert_regex(testOb->query_save_path("tester"), "^/save/account/t/tester"),
        assert_regex(testOb->query_save_path("Tester"), "^/save/account/T/Tester"),
    }) :));
}

void test_exists () {
    expect("query_exists handles invalid accounts", (: ({
        assert_equal(testOb->query_exists(""), 0),
        assert_equal(testOb->query_exists("testaccountinvalid"), 0),
        assert_equal(testOb->query_exists("TESTACCOUNTINVALID"), 0),
    }) :));

    expect("query_exists handles valid accounts", (: ({
        // create a mock account file
        assert_equal(write_file("/save/account/a/accounttest.o", "accounttest.o mock save", 1), 1),
        assert_equal(testOb->query_exists("accounttest"), 1),
        assert_equal(rm("/save/account/a/accounttest.o"), 1),

        assert_equal(write_file("/save/account/A/Accounttest.o", "Accounttest.o mock save", 1), 1),
        assert_equal(testOb->query_exists("Accounttest"), 1),
        assert_equal(rm("/save/account/A/Accounttest.o"), 1),
    }) :));
}