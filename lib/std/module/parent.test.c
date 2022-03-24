inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/module/parent.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_parent () {
    expect("query_parent returns this_object", (: ({
        assert(testOb->query_parent(), "==", this_object()),
    }) :));
}