inherit M_TEST;

void test_attributes () {
    expect("setting and querying attributes are handled", (: ({
        assert_equal(testOb->query_attribute("build"), UNDEFINED),
        assert_equal(testOb->query_attribute("complexion"), UNDEFINED),
        assert_equal(testOb->query_attribute("eye"), UNDEFINED),
        assert_equal(testOb->query_attribute("hair"), UNDEFINED),
        assert_equal(testOb->query_attribute("height"), UNDEFINED),
        assert_equal(testOb->query_attributes(), ([ ])),

        testOb->set_attribute("build", "test build"),
        assert_equal(testOb->query_attribute("build"), "test build"),

        testOb->set_attribute("complexion", "test complexion"),
        assert_equal(testOb->query_attribute("complexion"), "test complexion"),

        testOb->set_attribute("eye", "test eye"),
        assert_equal(testOb->query_attribute("eye"), "test eye"),

        testOb->set_attribute("hair", "test hair"),
        assert_equal(testOb->query_attribute("hair"), "test hair"),

        testOb->set_attribute("height", "test height"),
        assert_equal(testOb->query_attribute("height"), "test height"),

        assert_equal(testOb->query_attributes(), ([ "build": "test build", "complexion": "test complexion", "eye": "test eye", "hair": "test hair", "height": "test height" ])),
    }) :));

    expect("setting invalid attributes is handled", (: ({
        assert_equal(testOb->query_attribute("bad attribute"), UNDEFINED),
        testOb->set_attribute("bad attribute", "bad test attribute"),
        assert_equal(testOb->query_attribute("bad attribute"), UNDEFINED),
    }) :));

    expect("null attributes are initialized", (: ({
        store_variable("__Attribute", UNDEFINED, testOb),
        assert_equal(testOb->query_attributes(), ([ ])),
    }) :));
}