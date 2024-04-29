inherit M_TEST;

void test_pickaxe () {
    expect("pickaxe should be setup with name and id", (: ({
        assert_equal(testOb->id("pickaxe"), 1),
    }) :));
}