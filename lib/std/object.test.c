inherit M_TEST;
/**
 * @var {"/std/object"} testOb
 */

void test_parser_applies () {
    expect("object handles parser applies", (: ({
        assert_equal(testOb->is_living(), 0),
    }) :));
}

void test_apply_id () {
    expect("object apply id behaves", (: ({
        assert_equal(testOb->id("test0"), 0),
        assert_equal(testOb->id("test1"), 0),
        assert_equal(testOb->id("test2"), 0),

        testOb->set_id(({ "test1", "test2" })),
        assert_equal(testOb->id("test0"), 0),
        assert_equal(testOb->id("test1"), 1),
        assert_equal(testOb->id("test2"), 1),
    }) :));
}