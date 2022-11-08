inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/class.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_class () {
    expect_function("set_class", testOb);
    expect_function("query_class", testOb);

    expect("class settable and queryable", (: ({
        assert(testOb->query_class(), "==", "adventurer"),

        testOb->set_class("warrior"),
        assert(testOb->query_class(), "==", "warrior"),

        testOb->set_class("scoundrel"),
        assert(testOb->query_class(), "==", "scoundrel"),

        testOb->set_class(0),
        assert(testOb->query_class(), "==", "adventurer"),

        testOb->set_class("psionist"),
        assert(testOb->query_class(), "==", "psionist"),
    }) :));
}

void test_subclass () {
    expect_function("set_subclass", testOb);
    expect_function("query_subclass", testOb);

    expect("subclass settable and queryable", (: ({
        assert(testOb->query_subclass(), "==", "none"),

        // set class and subclass
        testOb->set_class("psionist"),
        assert(testOb->query_class(), "==", "psionist"),
        assert(testOb->query_subclass(), "==", "none"),
        testOb->set_subclass("sorcerer"),
        assert(testOb->query_subclass(), "==", "sorcerer"),

        // subclass clears
        testOb->set_subclass(0),
        assert(testOb->query_subclass(), "==", "none"),
    }) :));

    expect("subclass clears on class change", (: ({
        // set class and subclass
        testOb->set_class("psionist"),
        testOb->set_subclass("sorcerer"),
        assert(testOb->query_class(), "==", "psionist"),
        assert(testOb->query_subclass(), "==", "sorcerer"),
        // class and subclass clears
        testOb->set_class(0),
        assert(testOb->query_class(), "==", "adventurer"),
        assert(testOb->query_subclass(), "==", "none"),
    }) :));
}