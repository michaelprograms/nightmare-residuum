inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/module/parent.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_query_parent () {
    expect_true(testOb->query_parent() == this_object(), "query_parent returns this object");
}