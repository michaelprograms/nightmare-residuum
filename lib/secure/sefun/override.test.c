inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/secure/sefun/override.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_disabled_efuns () {
    expect_function("input_to", testOb);
    expect_function("this_player", testOb);

    expect("input_to throws error", (: ({
        assert_catch((: input_to() :), "*efun::input_to disabled\n"),
    }) :));
    expect("this_player throws error", (: ({
        assert_catch((: this_player() :), "*efun::this_player disabled\n"),
    }) :));
}

void test_efuns () {
    expect_function("users", testOb);

    expect("users returns an array", (: ({
        assert_equal(arrayp(testOb->users()), 1),
    }) :));
}