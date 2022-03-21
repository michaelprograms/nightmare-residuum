inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/override");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_efuns () {
    expect_function("input_to", testOb);

    expect("input_to throws error", (: ({
        assert((: input_to() :), "catch", "*efun::input_to disabled\n"),
    }) :));
    expect("this_player throws error", (: ({
        assert((: this_player() :), "catch", "*efun::this_player disabled\n"),
    }) :));
}