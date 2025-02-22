inherit M_TEST;

/**
 * @var {"/std/module/senses"} testOb
 */

void test_listen () {
    function fn = function () { return "Function sound."; };

    expect("null listens are initialized", (: ({
        assert_equal(testOb->query_listens(), ([ ])),
        store_variable("__Listens", UNDEFINED, testOb),
        assert_equal(testOb->query_listens(), ([ ])),
    }) :));

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

    expect("null smells are initialized", (: ({
        assert_equal(testOb->query_smells(), ([ ])),
        store_variable("__Smells", UNDEFINED, testOb),
        assert_equal(testOb->query_smells(), ([ ])),
    }) :));

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