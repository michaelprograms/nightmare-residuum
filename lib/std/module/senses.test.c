inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/senses.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_listen () {
    function fn = function () { return "Function sound."; };

    expect_function("set_listen", testOb);
    expect_function("query_listen", testOb);

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

    expect_function("set_smell", testOb);
    expect_function("query_smell", testOb);

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