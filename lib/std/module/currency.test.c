inherit M_TEST;

/**
 * @var {"/std/module/currency"} testOb
 */

void test_null_currencies () {
    expect("null achievements are initialized", (: ({
        assert_equal(testOb->query_currencies(), ({ })),
        store_variable("__Currency", UNDEFINED, testOb),
        assert_equal(testOb->query_currencies(), ({ })),
    }) :));
}

void test_currencies () {
    expect("query_currency returns -1 for invalid currency", (: ({
        assert_equal(testOb->query_currency("unknown"), -1),
        assert_equal(testOb->query_currency("invalid"), -1),
    }) :));
    expect("currency handles adding and querying", (: ({
        assert_equal(testOb->query_currencies(), ({ })),
        assert_equal(testOb->query_currency("copper"), 0),

        assert_equal(testOb->add_currency("copper", -10), 0), // can't remove
        assert_equal(testOb->query_currencies(), ({ })),
        assert_equal(testOb->query_currency("copper"), 0), // still 0

        assert_equal(testOb->add_currency("copper", 10), 1), // add
        assert_equal(testOb->query_currencies(), ({ "copper" })),
        assert_equal(testOb->query_currency("copper"), 10),

        assert_equal(testOb->add_currency("copper", -100), 0), // can't remove
        assert_equal(testOb->query_currencies(), ({ "copper" })),
        assert_equal(testOb->query_currency("copper"), 10), // still 10

        assert_equal(testOb->add_currency("copper", -5), 1), // can remove
        assert_equal(testOb->query_currencies(), ({ "copper" })), // some remains
        assert_equal(testOb->query_currency("copper"), 5),
    }) :));
    expect("currency handles bad inputs", (: ({
        assert_catch((: testOb->query_currency(UNDEFINED) :), "*Bad argument 1 to currency->query_currency\n"),

        assert_catch((: testOb->add_currency(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to currency->add_currency\n"),
        assert_catch((: testOb->add_currency("copper", UNDEFINED) :), "*Bad argument 2 to currency->add_currency\n"),
    }) :));
}