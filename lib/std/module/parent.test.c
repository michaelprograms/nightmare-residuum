inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/module/parent.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_parent () {
    expect("query_parent returns this_object", (: ({
        assert(testOb->query_parent(), "==", this_object()),
    }) :));
}

void test_set_parent () {
    object ob = new ("/std/object.c");

    expect("set_parent returns new object", (: ({
        // invalid object
        assert(testOb->set_parent($(ob)), "==", 0),
        // valid object (for testing)
        assert(testOb->set_parent(this_object()), "==", 1),
    }) :));

    destruct(ob);
}