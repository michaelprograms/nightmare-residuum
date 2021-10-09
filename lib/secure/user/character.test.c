inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    // testOb = clone_object("/secure/user/character");
}
void after_all_tests () {
    // destruct(testOb);
}

// @TODO tests