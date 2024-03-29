inherit M_TEST;
inherit STD_STORAGE;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/injectable.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_is_injectable () {
    expect_function("is_injectable", testOb);

    expect("is_injectable returns true", (: ({
        assert_equal(testOb->is_item(), 1),
        assert_equal(testOb->is_injectable(), 1),
        assert_equal(testOb->is_consumable(), 0),
        assert_equal(testOb->is_drink(), 0),
        assert_equal(testOb->is_food(), 0),
    }) :));
}

void test_strength () {
    expect_function("query_strength", testOb);
    expect_function("set_strength", testOb);

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

void test_type () {
    expect_function("query_type", testOb);
    expect_function("set_type", testOb);

    expect("injectable handles type", (: ({
        assert_equal(testOb->query_type(), ""),
        testOb->set_type("healing nanites"),
        assert_equal(testOb->query_type(), "healing nanites"),
    }) :));
}

void test_item_verb_inject_applies () {
    expect_function("direct_inject_obj", testOb);

    expect("inject handles verb apply direct_inject_obj", (: ({
        assert_equal(environment(testOb), 0),
        assert_equal(testOb->direct_inject_obj(), 0),
        assert_equal(testOb->handle_move(this_object()), 1),
        assert_equal(environment(testOb), this_object()),
        assert_equal(testOb->direct_inject_obj(), 1),
    }) :));
}

void test_handle_inject () {
    object character;

    expect_function("handle_inject", testOb);

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