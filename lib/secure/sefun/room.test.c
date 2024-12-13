inherit M_TEST;
inherit STD_LIVING;

/**
 * @var {"/secure/sefun/room"} testOb
 */

void test_describe_living_item () {
    expect("living items are described", (: ({
        assert_equal(testOb->describe_living_item(this_object()), ""),

        set_short("Test%^DEFAULT%^Short"),
        assert_equal(testOb->describe_living_item(this_object()), "Test%^I_RED%^BOLD%^Short"),

        set_short("Test Short"),
        assert_equal(testOb->describe_living_item(this_object()), "Test Short"),

        set_defeated(1),
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (defeated)"),
        set_defeated(0),

        __Vitals["hpMax"] = 100,
        __Vitals["hp"] = 100,
        assert_equal(testOb->describe_living_item(this_object()), "Test Short"),
        __Vitals["hp"] = 75,
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (bruised)"),
        __Vitals["hp"] = 50,
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (injured)"),
        __Vitals["hp"] = 25,
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (bleeding)"),
        __Vitals["hp"] = 0,
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (dying)"),
        __Vitals["hp"] = -25,
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (dying)"),

        __Vitals["hp"] = 100,
        set_posture("sitting"),
        assert_equal(testOb->describe_living_item(this_object()), "Test Short (sitting)"),
    }) :));
}