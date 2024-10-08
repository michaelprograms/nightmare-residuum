inherit M_TEST;

void test_character_name () {
    expect("character name sets up character", (: ({
        assert_equal(testOb->query_character(), UNDEFINED),

        testOb->set_character_name("test"),
        assert_equal(testOb->query_character()->is_character(), 1),

        testOb->query_character()->handle_remove(),
        assert_equal(testOb->query_character(), UNDEFINED),
    }) :));
}