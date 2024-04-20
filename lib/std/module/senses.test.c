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

void test_listen () {
    function fn = function () { return "Function sound."; };

    expect("listens are settable and queryable", (: ({
        // verify initial default
        assert_equal(testOb->query_listen("default"), UNDEFINED),
        // verify default
        testOb->set_listen("default", "Default sounds."),
        assert_equal(testOb->query_listen("default"), "Default sounds."),
        // verify non-default
        testOb->set_listen("something", "Some noise."),
        assert_equal(testOb->query_listen("something"), "Some noise."),
        // verify function
        testOb->set_listen("fn", $(fn)),
        assert_equal(testOb->query_listen("fn"), $(fn)),
    }) :));
}

void test_smell () {
    function fn = function () { return "Function scent."; };

    expect("smells are settable and queryable", (: ({
        // verify initial default
        assert_equal(testOb->query_smell("default"), UNDEFINED),
        // verify default
        testOb->set_smell("default", "Default scents."),
        assert_equal(testOb->query_smell("default"), "Default scents."),
        // verify non-default
        testOb->set_smell("something", "Some smell."),
        assert_equal(testOb->query_smell("something"), "Some smell."),
        // verify function
        testOb->set_smell("fn", $(fn)),
        assert_equal(testOb->query_smell("fn"), $(fn)),
    }) :));
}