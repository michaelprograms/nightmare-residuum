inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/location.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_last_location () {
    expect_function("query_last_location", testOb);
    expect_function("set_last_location", testOb);

    expect("handles setting and querying last location", (: ({
    assert(testOb->query_last_location(), "==", "/domain/Nowhere/room/void.c"), // defaults to void
    assert(testOb->set_last_location("a location"), "==", 0),
    assert(testOb->query_last_location(), "==", "a location"),
    assert(testOb->set_last_location("new location"), "==", 0),
    assert(testOb->query_last_location(), "==", "new location"),
    }) :));

    expect("set_last_location handles invalid argument 1", (: ({
        assert((: testOb->set_last_location(0) :), "catch", "*Bad argument 1 to location->set_last_location\n"),
        assert((: testOb->set_last_location(0.0) :), "catch", "*Bad argument 1 to location->set_last_location\n"),
        assert((: testOb->set_last_location("") :), "catch", "*Bad argument 1 to location->set_last_location\n"),
        assert((: testOb->set_last_location(({})) :), "catch", "*Bad argument 1 to location->set_last_location\n"),
        assert((: testOb->set_last_location(([])) :), "catch", "*Bad argument 1 to location->set_last_location\n"),
        assert((: testOb->set_last_location((: users :)) :), "catch", "*Bad argument 1 to location->set_last_location\n"),
    }) :));
}