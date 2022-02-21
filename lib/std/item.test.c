inherit M_TEST;
inherit STD_CONTAINER;

private nosave object testOb;
void before_each_test () {
    if (!testOb) testOb = clone_object("/std/item.c");
    this_object()->handle_move("/domain/Nowhere/void.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}

void test_is_item () {
    expect_function("is_item", testOb);
    expect_true(testOb->is_item(), "item->is_item returns true");
}

void test_item_verb_look_applies () {
    mixed *values = ({}), *results = ({});
    object r;

    r = new(STD_ROOM);

    expect_function("direct_look_at_obj", testOb);
    expect_function("direct_look_obj", testOb);

    values += ({ environment(testOb) });
    results += ({ 0 });
    values += ({ environment(this_object()) });
    results += ({ find_object("/domain/Nowhere/void.c") });

    values += ({ testOb->direct_look_at_obj() });
    results += ({ 0 });
    values += ({ testOb->direct_look_obj() });
    results += ({ 0 });

    values += ({ testOb->handle_move(r) });
    results += ({ 1 });
    values += ({ environment(testOb) });
    results += ({ r });
    values += ({ this_object()->handle_move(r) });
    results += ({ 1 });
    values += ({ environment(this_object()) });
    results += ({ r });

    values += ({ testOb->direct_look_at_obj() });
    results += ({ 1 });
    values += ({ testOb->direct_look_obj() });
    results += ({ 1 });

    values += ({ testOb->handle_move("/domain/Nowhere/void.c") });
    results += ({ 1 });
    values += ({ this_object()->handle_move("/domain/Nowhere/void.c") });
    results += ({ 1 });

    expect_arrays_equal(values, results, "item handled verb apply direct_look_at_obj");

    if (r) destruct(r);
}

void test_item_verb_drop_applies () {
    mixed *values = ({}), *results = ({});

    expect_function("direct_drop_obj", testOb);

    values += ({ environment(testOb) });
    results += ({ 0 });
    values += ({ environment(this_object()) });
    results += ({ find_object("/domain/Nowhere/void.c") });

    values += ({ testOb->direct_drop_obj() });
    results += ({ 0 }); // can't drop item yet

    values += ({ testOb->handle_move(this_object()) });
    results += ({ 1 });
    values += ({ environment(testOb) });
    results += ({ this_object() });

    values += ({ testOb->direct_drop_obj() });
    results += ({ 1 }); // can drop item

    values += ({ testOb->handle_move("/domain/Nowhere/void.c") });
    results += ({ 1 });

    expect_arrays_equal(values, results, "item handled verb apply direct_drop_obj");
}

void test_item_verb_give_applies () {
    mixed *values = ({}), *results = ({});

    expect_function("direct_give_obj_to_liv", testOb);

    values += ({ environment(testOb) });
    results += ({ 0 });

    values += ({ testOb->direct_give_obj_to_liv() });
    results += ({ 0 }); // can't give item yet

    values += ({ testOb->handle_move(this_object()) });
    results += ({ 1 });
    values += ({ environment(testOb) });
    results += ({ this_object() });

    values += ({ testOb->direct_give_obj_to_liv() });
    results += ({ 1 }); // can give item

    expect_arrays_equal(values, results, "item handled verb apply direct_give_obj_to_liv");
}

void test_item_verb_get_applies () {
    mixed *values = ({}), *results = ({});

    expect_function("direct_get_obj", testOb);

    values += ({ environment(testOb) });
    results += ({ 0 });
    values += ({ environment(this_object()) });
    results += ({ find_object("/domain/Nowhere/void.c") });

    values += ({ testOb->direct_get_obj() });
    results += ({ "You can't get what isn't here." });

    values += ({ testOb->handle_move("/domain/Nowhere/void.c") });
    results += ({ 1 });
    values += ({ environment(testOb) });
    results += ({ find_object("/domain/Nowhere/void.c") });

    values += ({ testOb->direct_get_obj() });
    results += ({ 1 }); // can get item

    // move item back
    values += ({ testOb->handle_move(this_object()) });
    results += ({ 1 });

    values += ({ testOb->direct_get_obj() });
    results += ({ 0 }); // already has item get item

    expect_arrays_equal(values, results, "item handled verb apply direct_get_obj");
}