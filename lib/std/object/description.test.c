inherit M_TEST;

void test_long_and_long_footer () {
    expect("long is settable and queryable", (: ({
        assert_equal(testOb->query_long(), ""),

        testOb->set_long("Test long"),
        assert_equal(testOb->query_long(), "Test long"),

        testOb->set_long((: base_name(previous_object(1)) :)),
        assert_equal(testOb->query_long(), base_name()),

        testOb->set_long(0),
        assert_equal(testOb->query_long(), ""),
    }) :));

    expect("long_footer is settable and queryable", (: ({
        assert_equal(testOb->query_long_footer(), UNDEFINED),

        testOb->set_long_footer("Test long"),
        assert_equal(testOb->query_long_footer(), "Test long"),

        testOb->set_long_footer((: base_name(previous_object(1)) :)),
        assert_equal(testOb->query_long_footer(), base_name()),

        testOb->set_long_footer(0),
        assert_equal(testOb->query_long_footer(), 0),
    }) :));
}

void test_short () {
    expect("short is settable and queryable", (: ({
        assert_equal(testOb->query_short(), ""),

        testOb->set_short("Test short"),
        assert_equal(testOb->query_short(), "Test short"),

        testOb->set_short((: base_name(previous_object(1)) :)),
        assert_equal(testOb->query_short(), base_name()),

        testOb->set_short(0),
        assert_equal(testOb->query_short(), ""),
    }) :));

    expect("short handles DEFAULT ANSI replacement", (: ({
        testOb->set_short("%^BLUE%^Blue text%^DEFAULT%^"),
        assert_equal(testOb->query_short(), "%^BLUE%^Blue text%^RESET%^"),
        assert_equal(testOb->query_short("%^RED%^"), "%^BLUE%^Blue text%^RED%^"),
    }) :));
}