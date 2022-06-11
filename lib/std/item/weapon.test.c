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
    return ({ "test_is_weapon", "test_type", "test_wielded", "test_item_verb_wield_applies", });
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
        assert(testOb->is_weapon(), "==", 1),
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_armor(), "==", UNDEFINED),
        assert(testOb->is_food(), "==", UNDEFINED),
    }) :));
}

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("weapon handles type", (: ({
        assert(testOb->query_type(), "==", 0),

        testOb->set_type("blade"),
        assert(testOb->query_type(), "==", "blade"),

        testOb->set_type("blunt"),
        assert(testOb->query_type(), "==", "blunt"),

        testOb->set_type("knife"),
        assert(testOb->query_type(), "==", "knife"),

        testOb->set_type("melee"),
        assert(testOb->query_type(), "==", "melee"),

        testOb->set_type("projectile"),
        assert(testOb->query_type(), "==", "projectile"),

        testOb->set_type("psionic"),
        assert(testOb->query_type(), "==", "psionic"),

        testOb->set_type("ranged"),
        assert(testOb->query_type(), "==", "ranged"),
    }) :));
}

void test_wielded () {
    expect_function("query_wielded", testOb);
    expect_function("set_wielded", testOb);
    expect_function("query_short", testOb);

    testOb->set_short("weapon");
    expect("weapon handles wielded", (: ({
        assert(testOb->query_wielded(), "==", 0),
        assert(testOb->query_short(), "==", "weapon"),

        testOb->set_wielded(this_object()),
        assert(testOb->query_wielded(), "==", this_object()),
        assert(testOb->query_short(), "==", "weapon (wielded)"),
    }) :));
}

void test_item_verb_wield_applies () {
    expect_function("direct_wield_obj", testOb);
    expect_function("direct_unwield_obj", testOb);

    __WieldableLimbs = ({ "left hand", "right hand" });
    __WieldedWeapons = ({ });

    expect("weapon handles verb applies direct_wield_obj and direct_unwield_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(testOb->direct_wield_obj(), "==", 0),
        assert(testOb->direct_unwield_obj(), "==", 0),

        assert(testOb->handle_move(this_object()), "==", 1),
        assert(environment(testOb), "==", this_object()),

        assert(testOb->direct_wield_obj(), "==", 1),
        assert(testOb->direct_unwield_obj(), "==", 0),

        __WieldedWeapons += ({ testOb }),
        testOb->set_wielded(this_object()),
        assert(testOb->direct_wield_obj(), "==", 0),
        assert(testOb->direct_unwield_obj(), "==", 1),
    }) :));

    __WieldableLimbs = 0;
    __WieldedWeapons = 0;
}