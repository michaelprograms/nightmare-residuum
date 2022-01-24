inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/std/living.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_living () {
    expect_function("is_living", testOb);

    expect_true(testOb->is_living(), "is_living handled");
}