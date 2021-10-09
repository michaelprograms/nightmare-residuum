inherit M_TEST;

private nosave object testOb;
void before_all_tests () {
    testOb = clone_object("/secure/sefun/override");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_efuns () {
    mixed err;
    expect_function("input_to", testOb);
    err = catch(input_to());
    expect_strings_equal(err, "*efun::input_to disabled\n", "input_to threw error");

    expect_function("this_player", testOb);
    err = catch(this_player());
    expect_strings_equal(err, "*efun::this_player disabled\n", "this_player threw error");
}