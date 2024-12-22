inherit M_TEST;

/**
 * @var {"/std/ability/weapons"} testOb
 */

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

private object *__WieldedWeapons = ({ });
object *query_wielded_weapons () {
    return __WieldedWeapons;
}

void test_best_weapon () {
    object w1 = new(STD_WEAPON), w2 = new(STD_WEAPON);

    w1->set_name("blade weapon");
    w1->set_type("blade");
    w1->set_wc(2);

    w2->set_name("blunt weapon");
    w2->set_type("blunt");
    w2->set_wc(1);

    expect("best weapon finds no weapons", (: ({
        assert_equal(testOb->query_best_weapon(this_object()), UNDEFINED),
    }) :));

    __WieldedWeapons = ({ w1 });
    expect("best weapon finds a weapon", (: ({
        assert_equal(testOb->query_best_weapon(this_object()), $(w1)),
    }) :));

    __WieldedWeapons = ({ w2 });
    expect("best weapon skips mismatch weapon types", (: ({
        testOb->set_weapons(([ "blade": ({ 1 }) ])),
        assert_equal(testOb->query_best_weapon(this_object()), UNDEFINED),
    }) :));
    expect("best weapon skips mismatch weapon hands", (: ({
        testOb->set_weapons(([ "blunt": ({ 2 }) ])),
        assert_equal(testOb->query_best_weapon(this_object()), UNDEFINED),
    }) :));

    __WieldedWeapons = ({ w1, w2 });
    expect("best weapon finds best weapon", (: ({
        testOb->set_weapons(([ ])),
        assert_equal(testOb->query_best_weapon(this_object()), $(w1)),
    }) :));

    if (w1) destruct(w1);
    if (w2) destruct(w2);
}