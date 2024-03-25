inherit M_TEST;
inherit STD_STORAGE;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item/weapon.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({
        "test_is_weapon",
        "test_type",
        "test_wielded",
        "test_item_verb_wield_applies",
        "text_item_verb_drop_applies",
    });
}

nosave private string *__WieldableLimbs;
string *query_wieldable_limbs () {
    return __WieldableLimbs;
}
nosave private object *__WieldedWeapons;
object *query_wielded_weapons () {
    return __WieldedWeapons;
}

void test_is_weapon () {
    expect_function("is_weapon", testOb);
    expect("is_weapon returns true", (: ({
        assert_equal(testOb->is_weapon(), 1),
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_armor(), UNDEFINED),
        assert_equal(testOb->is_food(), UNDEFINED),
    }) :));
}

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("weapon handles type", (: ({
        assert_equal(testOb->query_type(), 0),

        testOb->set_type("blade"),
        assert_equal(testOb->query_type(), "blade"),

        testOb->set_type("blunt"),
        assert_equal(testOb->query_type(), "blunt"),

        testOb->set_type("brawl"),
        assert_equal(testOb->query_type(), "brawl"),

        testOb->set_type("psionic"),
        assert_equal(testOb->query_type(), "psionic"),

        testOb->set_type("ranged"),
        assert_equal(testOb->query_type(), "ranged"),
    }) :));
}

void test_hands () {
    expect_function("query_hands", testOb);
    expect_function("set_hands", testOb);

    expect("weapon handles hands", (: ({
        assert_equal(testOb->query_hands(), 1),

        testOb->set_hands(2),
        assert_equal(testOb->query_hands(), 2),

        testOb->set_hands(1),
        assert_equal(testOb->query_hands(), 1),
    }) :));
}

void test_wc () {
    expect_function("query_wc", testOb);
    expect_function("set_wc", testOb);

    expect("weapon handles wc", (: ({
        assert_equal(testOb->query_wc(), 0),

        testOb->set_wc(1),
        assert_equal(testOb->query_wc(), 1),

        testOb->set_wc(5),
        assert_equal(testOb->query_wc(), 5),

        testOb->set_wc(10),
        assert_equal(testOb->query_wc(), 10),
    }) :));
}

void test_wielded () {
    expect_function("query_wielded", testOb);
    expect_function("set_wielded", testOb);
    expect_function("query_short", testOb);

    testOb->set_short("weapon");
    expect("weapon handles wielded", (: ({
        assert_equal(testOb->query_wielded(), 0),
        assert_equal(testOb->query_short(), "weapon"),

        testOb->set_wielded(this_object()),
        assert_equal(testOb->query_wielded(), this_object()),
        assert_equal(testOb->query_short(), "weapon (wielded)"),
    }) :));
}

void test_item_verb_wield_applies () {
    expect_function("direct_wield_obj", testOb);
    expect_function("direct_unwield_obj", testOb);

    __WieldableLimbs = ({ "left hand", "right hand" });
    __WieldedWeapons = ({ });

    expect("weapon handles verb applies direct_wield_obj and direct_unwield_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_wield_obj(), 0),
        assert_equal(testOb->direct_unwield_obj(), 0),

        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),

        assert_equal(testOb->direct_wield_obj(), 1),
        assert_equal(testOb->direct_unwield_obj(), 0),

        __WieldedWeapons += ({ testOb }),
        testOb->set_wielded(this_object()),
        assert_equal(testOb->direct_wield_obj(), 0),
        assert_equal(testOb->direct_unwield_obj(), 1),
    }) :));

    __WieldableLimbs = 0;
    __WieldedWeapons = 0;
}

void text_item_verb_drop_applies () {
    expect_function("direct_drop_obj", testOb);

    __WieldableLimbs = ({ "left hand", "right hand" });
    __WieldedWeapons = ({ });

    expect("weapon handles verb applies direct_drop_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_drop_obj(), 0),

        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),

        assert_equal(testOb->direct_drop_obj(), 1),

        __WieldedWeapons += ({ testOb }),
        testOb->set_wielded(this_object()),
        assert_equal(testOb->direct_drop_obj(), 0),
    }) :));

    __WieldableLimbs = 0;
    __WieldedWeapons = 0;
}