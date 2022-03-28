inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/living/body.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_gender () {
    expect_function("set_gender", testOb);
    expect_function("query_gender", testOb);

    expect("gender settable and queryable", (: ({
        assert(testOb->query_gender(), "==", "neither"),

        testOb->set_gender("male"),
        assert(testOb->query_gender(), "==", "male"),

        testOb->set_gender("female"),
        assert(testOb->query_gender(), "==", "female"),

        testOb->set_gender("neither"),
        assert(testOb->query_gender(), "==", "neither"),

        testOb->set_gender(0),
        assert(testOb->query_gender(), "==", "none"),

        testOb->set_gender("invalid"),
        assert(testOb->query_gender(), "==", "none"),
    }) :));
}

void test_level () {
    expect_function("set_level", testOb);
    expect_function("query_level", testOb);

    expect("level settable and queryable", (: ({
        assert(testOb->query_level(), "==", 0),

        testOb->set_level(123),
        assert(testOb->query_level(), "==", 123),

        testOb->set_level(0),
        assert(testOb->query_level(), "==", 0),

        testOb->set_level(-123),
        assert(testOb->query_level(), "==", 123),
    }) :));
}

void test_species () {
    expect_function("set_species", testOb);
    expect_function("query_species", testOb);

    expect("species settable and queryable", (: ({
        assert(testOb->query_species(), "==", "unknown"),

        testOb->set_species("human"),
        assert(testOb->query_species(), "==", "human"),

        testOb->set_species("robot"),
        assert(testOb->query_species(), "==", "robot"),

        assert((: testOb->set_species(0) :), "catch", "*Bad argument 1 to body->set_species\n"),
        assert((: testOb->set_species() :), "catch", "*Bad argument 1 to body->set_species\n"),

        assert(testOb->query_species(), "==", "robot"), // species was not changed
    }) :));
}