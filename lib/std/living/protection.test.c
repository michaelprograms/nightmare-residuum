inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}

void test_query_and_set_protection () {
    expect("protection is queryable", (: ({
        // initial values
        assert_equal(testOb->query_protection(), 0),

        assert_equal(testOb->set_protection(10), 10),
        assert_equal(testOb->query_protection(), 10),

        assert_equal(testOb->set_protection(123), 123),
        assert_equal(testOb->query_protection(), 123),
    }) :));
}
void test_add_protection () {
    expect("protection is addable", (: ({
        // initial value
        assert_equal(testOb->query_protection(), 0),
        // add
        assert_equal(testOb->add_protection(4, 0), 4),
        assert_equal(testOb->query_protection(), 4),
        // add
        assert_equal(testOb->add_protection(6, 0), 10),
        assert_equal(testOb->query_protection(), 10),
    }) :));
}

void test_remove_protection () {
    expect("protection is removable", (: ({
        // initial value
        assert_equal(testOb->add_protection(10, 0), 10),
        assert_equal(testOb->query_protection(), 10),
        // remove
        assert_equal(testOb->remove_protection(3), 7),
        assert_equal(testOb->query_protection(), 7),
        // remove
        assert_equal(testOb->remove_protection(6), 1),
        assert_equal(testOb->query_protection(), 1),
    }) :));
}

void test_clear_protection () {
    expect("protection is clearable", (: ({
        assert_equal(testOb->add_protection(10, 0), 10),
        assert_equal(testOb->query_protection(), 10),
        assert_equal(testOb->clear_protection(), 0),
        assert_equal(testOb->query_protection(), 0),
    }) :));
}

void test_handle_remove () {
    expect("protection is cleared upon handle_remove", (: ({
        assert_equal(testOb->add_protection(10, 0), 10),
        assert_equal(testOb->query_protection(), 10),
        assert_equal(testOb->handle_remove(), 0),
        assert_equal(testOb->query_protection(), 0),
    }) :));
}