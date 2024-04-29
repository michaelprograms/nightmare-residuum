inherit M_TEST;

void test_axe () {
    expect("axe should be setup with name and id", (: ({
        assert_equal(testOb->id("axe"), 1),
    }) :));
}