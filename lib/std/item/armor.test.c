inherit M_TEST;
inherit STD_STORAGE;

int query_can_wear_armor (object ob) {
    return 1;
}
nosave private object *__AllArmor;
object *query_all_armor () {
    return __AllArmor;
}

void test_is_armor () {
    expect("is_armor returns true", (: ({
        assert_equal(testOb->is_armor(), 1),
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_weapon(), UNDEFINED),
        assert_equal(testOb->is_food(), UNDEFINED),
    }) :));
}

void test_type () {
    expect("armor handles type", (: ({
        assert_equal(testOb->query_type(), 0),

        testOb->set_type("pants"),
        assert_equal(testOb->query_type(), "pants"),

        testOb->set_type("boots"),
        assert_equal(testOb->query_type(), "boots"),

        testOb->set_type("shirt"),
        assert_equal(testOb->query_type(), "shirt"),

        testOb->set_type("gloves"),
        assert_equal(testOb->query_type(), "gloves"),

        testOb->set_type("hat"),
        assert_equal(testOb->query_type(), "hat"),
    }) :));
}

void test_limbs () {
    expect("armor handles limbs", (: ({
        assert_equal(testOb->query_limbs(), 0),

        testOb->set_limbs(({ "left leg", "right leg" })),
        assert_equal(testOb->query_limbs(), ({ "left leg", "right leg" })),

        testOb->set_limbs(({ "left foot", "right foot" })),
        assert_equal(testOb->query_limbs(), ({ "left foot", "right foot" })),

        testOb->set_limbs(({ "torso", "left arm", "right arm" })),
        assert_equal(testOb->query_limbs(), ({ "torso", "left arm", "right arm" })),

        testOb->set_limbs(({ "left hand", "right hand" })),
        assert_equal(testOb->query_limbs(), ({ "left hand", "right hand" })),

        testOb->set_limbs(({ "head", })),
        assert_equal(testOb->query_limbs(), ({ "head", })),
    }) :));
}

void test_worn () {
    testOb->set_short("armor");
    expect("armor handles worn", (: ({
        assert_equal(testOb->query_worn(), 0),
        assert_equal(testOb->query_short(), "armor"),

        testOb->set_worn(this_object()),
        assert_equal(testOb->query_worn(), this_object()),
        assert_equal(testOb->query_short(), "armor (worn)"),
    }) :));
}

void test_ac () {
    expect("armor handles ac", (: ({
        assert_equal(testOb->query_ac(), 0),

        testOb->set_ac(1),
        assert_equal(testOb->query_ac(), 1),

        testOb->set_ac(5),
        assert_equal(testOb->query_ac(), 5),

        testOb->set_ac(10),
        assert_equal(testOb->query_ac(), 10),
    }) :));
}

void test_item_verb_wear_applies () {
    __AllArmor = ({ });

    expect("armor handles verb applies direct_wear_obj and direct_unwear_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_wear_obj(), 0),
        assert_equal(testOb->direct_unwear_obj(), 0),

        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),

        assert_equal(testOb->direct_wear_obj(), 1),
        assert_equal(testOb->direct_unwear_obj(), 0),

        __AllArmor += ({ testOb }),
        testOb->set_worn(this_object()),
        assert_equal(testOb->direct_wear_obj(), 0),
        assert_equal(testOb->direct_unwear_obj(), 1),

        testOb->remove_worn(this_object()),
        assert_equal(testOb->query_worn(), 0),
        __AllArmor -= ({ testOb }),
    }) :));

    __AllArmor = 0;
}

void test_item_verb_drop_applies () {
    __AllArmor = ({ });

    expect("armor handles verb apply direct_drop_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_drop_obj(), 0),

        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),

        assert_equal(testOb->direct_drop_obj(), 1),

        __AllArmor += ({ testOb }),
        testOb->set_worn(this_object()),
        assert_equal(testOb->direct_drop_obj(), 0),
    }) :));

    __AllArmor = 0;
}