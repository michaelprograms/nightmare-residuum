inherit M_TEST;
inherit STD_STORAGE;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item/armor.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_is_armor", "test_type", "test_limbs", "test_worn", "test_ac", "test_item_verb_wear_applies", });
}

int query_can_wear_armor (object ob) {
    return 1;
}
nosave private object *__AllArmor;
object *query_all_armor () {
    return __AllArmor;
}

void test_is_armor () {
    expect_function("is_armor", testOb);
    expect("is_armor returns true", (: ({
        assert(testOb->is_armor(), "==", 1),
        assert(testOb->is_item(), "==", 1),
        assert(testOb->is_weapon(), "==", UNDEFINED),
        assert(testOb->is_food(), "==", UNDEFINED),
    }) :));
}

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("armor handles type", (: ({
        assert(testOb->query_type(), "==", 0),

        testOb->set_type("pants"),
        assert(testOb->query_type(), "==", "pants"),

        testOb->set_type("boots"),
        assert(testOb->query_type(), "==", "boots"),

        testOb->set_type("shirt"),
        assert(testOb->query_type(), "==", "shirt"),

        testOb->set_type("gloves"),
        assert(testOb->query_type(), "==", "gloves"),

        testOb->set_type("hat"),
        assert(testOb->query_type(), "==", "hat"),
    }) :));
}

void test_limbs () {
    expect_function("query_limbs", testOb);
    expect_function("set_limbs", testOb);

    expect("armor handles limbs", (: ({
        assert(testOb->query_limbs(), "==", 0),

        testOb->set_limbs(({ "left leg", "right leg" })),
        assert(testOb->query_limbs(), "==", ({ "left leg", "right leg" })),

        testOb->set_limbs(({ "left foot", "right foot" })),
        assert(testOb->query_limbs(), "==", ({ "left foot", "right foot" })),

        testOb->set_limbs(({ "torso", "left arm", "right arm" })),
        assert(testOb->query_limbs(), "==", ({ "torso", "left arm", "right arm" })),

        testOb->set_limbs(({ "left hand", "right hand" })),
        assert(testOb->query_limbs(), "==", ({ "left hand", "right hand" })),

        testOb->set_limbs(({ "head", })),
        assert(testOb->query_limbs(), "==", ({ "head", })),
    }) :));
}

void test_worn () {
    expect_function("query_worn", testOb);
    expect_function("set_worn", testOb);
    expect_function("query_short", testOb);

    testOb->set_short("armor");
    expect("armor handles worn", (: ({
        assert(testOb->query_worn(), "==", 0),
        assert(testOb->query_short(), "==", "armor"),

        testOb->set_worn(this_object()),
        assert(testOb->query_worn(), "==", this_object()),
        assert(testOb->query_short(), "==", "armor (worn)"),
    }) :));
}

void test_ac () {
    expect_function("query_ac", testOb);
    expect_function("set_ac", testOb);

    expect("armor handles ac", (: ({
        assert(testOb->query_ac(), "==", 0),

        testOb->set_ac(1),
        assert(testOb->query_ac(), "==", 1),

        testOb->set_ac(5),
        assert(testOb->query_ac(), "==", 5),

        testOb->set_ac(10),
        assert(testOb->query_ac(), "==", 10),
    }) :));
}

void test_item_verb_wear_applies () {
    expect_function("direct_wear_obj", testOb);
    expect_function("direct_unwear_obj", testOb);

    __AllArmor = ({ });

    expect("armor handles verb applies direct_wear_obj and direct_unwear_obj", (: ({
        assert(environment(testOb), "==", 0),
        assert(testOb->direct_wear_obj(), "==", 0),
        assert(testOb->direct_unwear_obj(), "==", 0),

        assert(testOb->handle_move(this_object()), "==", 1),
        assert(environment(testOb), "==", this_object()),

        assert(testOb->direct_wear_obj(), "==", 1),
        assert(testOb->direct_unwear_obj(), "==", 0),

        __AllArmor += ({ testOb }),
        testOb->set_worn(this_object()),
        assert(testOb->direct_wear_obj(), "==", 0),
        assert(testOb->direct_unwear_obj(), "==", 1),
    }) :));

    __AllArmor = 0;
}