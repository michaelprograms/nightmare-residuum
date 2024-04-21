inherit M_TEST;

void test_query_parent () {
    expect("query_parent returns this_object", (: ({
        assert_equal(testOb->query_parent(), this_object()),
    }) :));
}

void test_set_parent () {
    object ob = new ("/std/object.c");

    expect("set_parent returns new object", (: ({
        // invalid object
        assert_equal(testOb->set_parent($(ob)), 0),
        // valid object (for testing)
        assert_equal(testOb->set_parent(this_object()), 1),
    }) :));

    destruct(ob);
}