inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/location.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_last_location () {
    string *values = ({}), *results = ({});

    expect_function("query_last_location", testOb);
    expect_function("set_last_location", testOb);

    // test adding and removing
    values += ({ testOb->query_last_location() });
    results += ({ "/domain/Nowhere/room/void.c" }); // defaults to void
    testOb->set_last_location("a location");
    values += ({ testOb->query_last_location() });
    results += ({ "a location" });
    testOb->set_last_location("new location");
    values += ({ testOb->query_last_location() });
    results += ({ "new location" });

    expect_arrays_equal(values, results, "handled setting and querying last location");

    expect_catches (({
        (: testOb->set_last_location(0) :),
        (: testOb->set_last_location(0.0) :),
        (: testOb->set_last_location("") :),
        (: testOb->set_last_location(({})) :),
        (: testOb->set_last_location(([])) :),
        (: testOb->set_last_location((: users :)) :),
    }), "*Bad argument 1 to location->set_last_location\n", "set_last_location handled invalid argument 1");
}