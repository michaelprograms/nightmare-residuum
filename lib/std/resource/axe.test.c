inherit M_TEST;

/**
 * @var {"/std/resource/harvestable"} testOb
 */

void test_axe () {
    expect("axe should be setup with name and id", (: ({
        assert_equal(testOb->id("axe"), 1),
    }) :));
}