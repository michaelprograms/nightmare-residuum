inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/attribute.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_attributes () {
    expect_function("query_attributes", testOb);
    expect_function("query_attribute", testOb);
    expect_function("set_attribute", testOb);

    expect("setting and querying attributes are handled", (: ({
        assert(testOb->query_attribute("build"), "==", UNDEFINED),
        assert(testOb->query_attribute("complexion"), "==", UNDEFINED),
        assert(testOb->query_attribute("eye"), "==", UNDEFINED),
        assert(testOb->query_attribute("hair"), "==", UNDEFINED),
        assert(testOb->query_attribute("height"), "==", UNDEFINED),

        testOb->set_attribute("build", "test build"),
        assert(testOb->query_attribute("build"), "==", "test build"),

        testOb->set_attribute("complexion", "test complexion"),
        assert(testOb->query_attribute("complexion"), "==", "test complexion"),

        testOb->set_attribute("eye", "test eye"),
        assert(testOb->query_attribute("eye"), "==", "test eye"),

        testOb->set_attribute("hair", "test hair"),
        assert(testOb->query_attribute("hair"), "==", "test hair"),

        testOb->set_attribute("height", "test height"),
        assert(testOb->query_attribute("height"), "==", "test height"),
    }) :));
}