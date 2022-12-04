inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/biography.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_experience", "test_handle_victory", "test_handle_defeat", });
}

nosave private int __MockLevel, __MockLiving;
int query_level () { return __MockLevel; }
int is_living () { return __MockLiving; }

void test_experience () {
    expect_function("add_experience", testOb);
    expect_function("spend_experience", testOb);
    expect_function("query_experience", testOb);
    expect_function("query_total_experience", testOb);

    expect("handles adding and spending experience", (: ({
        assert(testOb->query_experience(), "==", 0),
        assert(testOb->query_total_experience(), "==", 0),

        assert(testOb->add_experience(123), "==", 0),
        assert(testOb->query_experience(), "==", 123),
        assert(testOb->query_total_experience(), "==", 0),

        assert(testOb->add_experience(9000), "==", 0),
        assert(testOb->query_experience(), "==", 9123),
        assert(testOb->query_total_experience(), "==", 0),

        assert(testOb->spend_experience(1000), "==", 0),
        assert(testOb->query_experience(), "==", 8123),
        assert(testOb->query_total_experience(), "==", 1000),

        assert(testOb->spend_experience(2000), "==", 0),
        assert(testOb->query_experience(), "==", 6123),
        assert(testOb->query_total_experience(), "==", 3000),
    }) :));

    expect("add_experience handled bad argument 1", (: ({
        assert((:  testOb->add_experience(this_object()) :), "catch", "*Bad argument 1 to body->add_experience\n"),
        assert((:  testOb->add_experience(1.0) :), "catch", "*Bad argument 1 to body->add_experience\n"),
        assert((:  testOb->add_experience("") :), "catch", "*Bad argument 1 to body->add_experience\n"),
        assert((:  testOb->add_experience(({})) :), "catch", "*Bad argument 1 to body->add_experience\n"),
        assert((:  testOb->add_experience(([])) :), "catch", "*Bad argument 1 to body->add_experience\n"),
        assert((:  testOb->add_experience((: 1 :)) :), "catch", "*Bad argument 1 to body->add_experience\n"),
    }) :));

    expect("spend_experience handled bad argument 1", (: ({
        assert((: testOb->spend_experience(10000) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((: testOb->spend_experience(0) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((: testOb->spend_experience(-10000) :), "catch", "*Bad argument 1 to body->spend_experience\n"),

        assert((:  testOb->spend_experience(this_object()) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((:  testOb->spend_experience(1.0) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((:  testOb->spend_experience("") :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((:  testOb->spend_experience(({})) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((:  testOb->spend_experience(([])) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
        assert((:  testOb->spend_experience((: 1 :)) :), "catch", "*Bad argument 1 to body->spend_experience\n"),
    }) :));
}

void test_handle_victory () {
    expect_function("handle_victory", testOb);
    expect_function("query_victory", testOb);

    // setup test object
    __MockLiving = 1;
    __MockLevel = 1;

    expect("handle_victory behaves", (: ({
        assert(testOb->query_experience(), "==", 0),
        assert(testOb->query_victory(), "==", 0),
        assert(testOb->handle_victory(this_object()), "==", 0),
        assert(testOb->query_experience() > 0, "==", 1),
        assert(testOb->query_victory(), "==", 1),
    }) :));

    // cleanup
    __MockLiving = 0;
    __MockLevel = 0;
}

void test_handle_defeat () {
    object r = new(STD_ROOM);

    expect_function("handle_defeat", testOb);
    expect_function("query_defeat", testOb);

    // setup test object
    if (testOb) destruct(testOb);
    testOb = new(STD_LIVING); // need biography's parent inherit living for handle_move

    expect("handle_defeat behaved", (: ({
        assert(testOb->query_experience(), "==", 0),
        assert(testOb->query_defeat(), "==", 0),
        assert(testOb->handle_move($(r)), "==", 1),
        assert(sizeof($(r)->query_living_contents()), "==", 1),
        assert(sizeof($(r)->query_item_contents()), "==", 0),

        // test defeat with keep
        assert(testOb->handle_defeat(1), "==", 0),
        assert(objectp(testOb), "==", 1),
        assert(testOb->query_defeat(), "==", 1),
        assert(sizeof($(r)->query_living_contents()), "==", 0),
        assert(sizeof($(r)->query_item_contents()), "==", 1),
        assert(!!present("corpse", $(r)), "==", 1),
        assert(present("corpse", $(r))->handle_remove(), "==", 1),

        // move test object back
        assert(testOb->handle_move($(r)), "==", 1),
        assert(sizeof($(r)->query_living_contents()), "==", 1),
        assert(sizeof($(r)->query_item_contents()), "==", 0),

        // test defeat with keep
        assert(testOb->handle_defeat(0), "==", 0),
        assert(objectp(testOb), "==", 0),
        assert(sizeof($(r)->query_living_contents()), "==", 0),
        assert(sizeof($(r)->query_item_contents()), "==", 1),
        assert(!!present("corpse", $(r)), "==", 1),
        assert(present("corpse", $(r))->handle_remove(), "==", 1),

        // cleanup
        $(r) && destruct($(r)),
    }) :));
}