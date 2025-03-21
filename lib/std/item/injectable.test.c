inherit M_TEST;
inherit STD_STORAGE;

/**
 * @var {"/std/item/injectable"} testOb
 */

void test_is_injectable () {
    expect("is_injectable returns true", (: ({
        assert_equal(itemp(testOb), 1),
        assert_equal(testOb->is_injectable(), 1),
        assert_equal(drinkp(testOb), 0),
        assert_equal(foodp(testOb), 0),
    }) :));
}

void test_strength () {
    expect("injectable handles strength", (: ({
        assert_equal(testOb->query_strength(), 0),
        testOb->set_strength(5),
        assert_equal(testOb->query_strength(), 5),
        testOb->set_strength(100),
        assert_equal(testOb->query_strength(), 100),
        testOb->set_strength(0),
        assert_equal(testOb->query_strength(), 0),
        testOb->set_strength(-5),
        assert_equal(testOb->query_strength(), -5),
    }) :));
}

void test_item_verb_inject_applies () {
    expect("inject handles verb apply direct_inject_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_inject_obj(), 0),
        assert_equal(testOb->direct_inject_obj_into_liv(), 0),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_inject_obj(), 1),
        assert_equal(testOb->direct_inject_obj_into_liv(), 1),
    }) :));
}

void test_handle_inject () {
    object character;

    // create test items
    character = new(STD_CHARACTER);

    expect("handle_inject behaves", (: ({
        // setup injectable
        testOb->set_type("healing nanites"),
        assert_equal(testOb->query_type(), "healing nanites"),
        testOb->set_strength(123),
        assert_equal(testOb->query_strength(), 123),

        // setup test character
        assert_equal(testOb->handle_move($(character)), 1),
        assert_equal($(character)->query_injections(), ([ ])),

        // test eating
        assert_equal(testOb->handle_inject($(character)), 0),
        assert_equal($(character)->query_injections(), ([ "healing nanites": 123, ])),

        assert_equal(objectp(testOb), 0), // injectable was removed
    }) :));

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");

    if (character) destruct(character);
}