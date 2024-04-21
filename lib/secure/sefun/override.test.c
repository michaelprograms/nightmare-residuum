inherit M_TEST;

void test_disabled_efuns () {
    expect("input_to throws error", (: ({
        assert_catch((: input_to() :), "*efun::input_to disabled\n"),
    }) :));
    expect("this_player throws error", (: ({
        assert_catch((: this_player() :), "*efun::this_player disabled\n"),
    }) :));
}

void test_efuns () {
    expect("users returns an array", (: ({
        assert_equal(arrayp(testOb->users()), 1),
    }) :));
}