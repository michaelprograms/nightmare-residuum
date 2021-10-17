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

    expect_catches (({
        (: input_to() :),
    }), "*efun::input_to disabled\n", "input_to threw error");

    expect_catches (({
        (: this_player() :),
    }), "*efun::this_player disabled\n", "this_player() threw error");
}