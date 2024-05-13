inherit M_TEST;

void test_ability_requirements () {
    expect("", (: ({
        assert_equal(testOb->query_ability_requirements(), UNDEFINED),

        testOb->set_ability_requirements(([ ])),
        assert_equal(testOb->query_ability_requirements(), ([ ])),

        testOb->set_ability_requirements(([ "level": 123, ])),
        assert_equal(testOb->query_ability_requirements(), ([ "level": 123 ])),
    }) :));
}