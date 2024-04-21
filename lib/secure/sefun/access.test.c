inherit M_TEST;

void test_access () {
    expect("unguarded evaluates function", (: ({
        assert_equal(testOb->unguarded((: MAX_INT :)), MAX_INT),
        assert_equal(testOb->unguarded(function () { return MAX_INT; }), MAX_INT),
    }) :));
}