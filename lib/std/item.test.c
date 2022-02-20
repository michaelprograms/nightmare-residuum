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
    results += ({ 0 });

    values += ({ testOb->handle_move(this_object()) });
    results += ({ 1 });
    values += ({ environment(testOb) });
    results += ({ this_object() });

    values += ({ testOb->direct_drop_obj() });
    results += ({ 1 });

    values += ({ testOb->handle_move("/domain/Nowhere/void.c") });
    results += ({ 1 });

    expect_arrays_equal(values, results, "item handled verb apply direct_drop_obj");
}