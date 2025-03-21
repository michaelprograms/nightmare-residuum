inherit M_TEST;

/**
 * @var {"/std/resource/resource"} testOb
 */

void test_resource () {
    expect("resource is cloneable", (: ({
        assert_equal(clonep(testOb), 1),
        assert_equal(testOb->query_name(), "resource"),
        assert_equal(testOb->is_resource(), 1),
    }) :));
}