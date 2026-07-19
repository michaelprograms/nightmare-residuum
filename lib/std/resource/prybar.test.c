inherit M_TEST;

/**
 * @var {"/std/resource/prybar"} testOb
 */

void test_prybar() {
    expect("prybar should be setup with name and id", (: ({
        assert_equal(testOb->id("prybar"), 1),
    }) :));
}
