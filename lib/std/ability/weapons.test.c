inherit M_TEST;

void test_weapons () {
    expect("set_weapons handles bad inputs", (: ({
        assert_catch((: testOb->set_weapons(UNDEFINED) :), "*Bad argument 1 to weapon->set_weapons\n"),
        assert_catch((: testOb->set_weapons(0) :), "*Bad argument 1 to weapon->set_weapons\n"),

        assert_catch((: testOb->set_weapons(([ "bad": ([ ]) ])) :), "*Bad argument (keys) to weapon->set_weapons\n"),

        assert_catch((: testOb->set_weapons(([ "blade": ({ 0 }) ])) :), "*Bad argument (values) to weapon->set_weapons\n"),
        assert_catch((: testOb->set_weapons(([ "blade": ({ 3 }) ])) :), "*Bad argument (values) to weapon->set_weapons\n"),
    }) :));
    expect("weapons are settable and queryable", (: ({
        assert_equal(testOb->query_weapons(), ([ ])),

        testOb->set_weapons(([ "blade": ({ 1 }) ])),
        assert_equal(testOb->query_weapons(), ([ "blade": ({ 1 }), ])),

        testOb->set_weapons(([ "blade": ({ 1, 2 }) ])),
        assert_equal(testOb->query_weapons(), ([ "blade": ({ 1, 2 }), ])),
    }) :));
}