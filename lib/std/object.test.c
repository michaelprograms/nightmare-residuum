inherit M_TEST;

void test_lifecycle_functions () {
    // @TODO test these better
}

void test_parser_applies () {
    expect("object handles parser applies", (: ({
        assert_equal(testOb->is_living(), 0),
        assert_equal(testOb->inventory_visible(), 0),
        assert_equal(testOb->inventory_accessible(), 0),
    }) :));
}