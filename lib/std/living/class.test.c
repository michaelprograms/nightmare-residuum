inherit M_TEST;

/**
 * @var {"/std/living/class"} testOb
 */

void test_class () {
    expect("class settable and queryable", (: ({
        assert_equal(testOb->query_class(), "adventurer"),

        testOb->set_class("warrior"),
        assert_equal(testOb->query_class(), "warrior"),

        testOb->set_class("scoundrel"),
        assert_equal(testOb->query_class(), "scoundrel"),

        testOb->set_class(0),
        assert_equal(testOb->query_class(), "adventurer"),

        testOb->set_class("psionist"),
        assert_equal(testOb->query_class(), "psionist"),
    }) :));
}

void test_subclass () {
    expect("subclass settable and queryable", (: ({
        assert_equal(testOb->query_subclass(), "none"),

        // set class and subclass
        testOb->set_class("psionist"),
        assert_equal(testOb->query_class(), "psionist"),
        assert_equal(testOb->query_subclass(), "none"),
        testOb->set_subclass("sorcerer"),
        assert_equal(testOb->query_subclass(), "sorcerer"),

        // subclass clears
        testOb->set_subclass(0),
        assert_equal(testOb->query_subclass(), "none"),
    }) :));

    expect("subclass clears on class change", (: ({
        // set class and subclass
        testOb->set_class("psionist"),
        testOb->set_subclass("sorcerer"),
        assert_equal(testOb->query_class(), "psionist"),
        assert_equal(testOb->query_subclass(), "sorcerer"),
        // class and subclass clears
        testOb->set_class(0),
        assert_equal(testOb->query_class(), "adventurer"),
        assert_equal(testOb->query_subclass(), "none"),
    }) :));
}