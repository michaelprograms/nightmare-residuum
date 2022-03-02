inherit M_TEST;
inherit STD_CONTAINER;
inherit "/std/living/vitals";

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/food.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_is_food", "test_strength", "test_item_verb_eat_applies", }); // "test_handle_eat"
}

void test_is_food () {
    expect_function("is_food", testOb);
    expect_true(testOb->is_food(), "food->is_food returns true");
}

void test_strength () {
    mixed *values = ({}), *results = ({});

    expect_function("query_strength", testOb);
    expect_function("set_strength", testOb);

    values += ({ testOb->query_strength() });
    results += ({ 0 });

    testOb->set_strength(5);
    values += ({ testOb->query_strength() });
    results += ({ 5 });

    testOb->set_strength(100);
    values += ({ testOb->query_strength() });
    results += ({ 100 });

    testOb->set_strength(0);
    values += ({ testOb->query_strength() });
    results += ({ 0 });

    testOb->set_strength(-5);
    values += ({ testOb->query_strength() });
    results += ({ -5 });

    expect_arrays_equal(values, results, "food handled strength");
}

void test_handle_eat () {
    mixed *values = ({}), *results = ({});
    object r;

    r = new(STD_ROOM);

    expect_function("handle_eat", testOb);

    // setup food
    testOb->set_strength(5);
    values += ({ testOb->query_strength() });
    results += ({ 5 });

    // setup test object
    values += ({ this_object()->handle_move(r) });
    results += ({ 1 });
    this_object()->update_vitals(); // initialize vitals
    this_object()->add_hp(5);
    values += ({ this_object()->query_hp() });
    results += ({ 5 });
    this_object()->add_sp(5);
    values += ({ this_object()->query_sp() });
    results += ({ 5 });
    this_object()->add_mp(5);
    values += ({ this_object()->query_mp() });
    results += ({ 5 });

    // test eating
    testOb->handle_eat(this_object());
    values += ({ this_object()->query_hp() });
    results += ({ 10 });
    values += ({ this_object()->query_sp() });
    results += ({ 10 });
    values += ({ this_object()->query_mp() });
    results += ({ 10 });

    values += ({ objectp(testOb) });
    results += ({ 0 });

    expect_arrays_equal(values, results, "food handle_eat behaved");

    // cleanup
    this_object()->handle_move("/domain/Nowhere/room/void.c");
    if (r) destruct(r);
}

void test_item_verb_eat_applies () {
    mixed *values = ({}), *results = ({});

    expect_function("direct_eat_obj", testOb);

    values += ({ environment(testOb) });
    results += ({ 0 });
    values += ({ testOb->direct_eat_obj() });
    results += ({ 0 });

    testOb->handle_move(this_object());

    values += ({ environment(testOb) });
    results += ({ this_object() });
    values += ({ testOb->direct_eat_obj() });
    results += ({ 1 });

    expect_arrays_equal(values, results, "food handled verb apply direct_eat_obj");
}