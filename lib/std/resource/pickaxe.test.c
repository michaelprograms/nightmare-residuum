inherit M_TEST;

/**
 * @var {"/std/resource/harvestable"} testOb
 */

void test_pickaxe () {
    expect("pickaxe should be setup with name and id", (: ({
        assert_equal(testOb->id("pickaxe"), 1),
    }) :));
}