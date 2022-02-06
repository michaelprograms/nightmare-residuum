inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/object.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_lifecycle_functions () {
    // @TODO test these better

    expect_function("create", testOb);

    // expect_function("heart_beat", testOb);

    expect_function("id", testOb);
    // Tests for id handled by /std/module/id.test.c

    expect_function("move_or_destruct", testOb);

    expect_function("reset", testOb);

    // expect_function("virtual_start", testOb);
}

void test_move () {
    int *values = ({}), *results = ({});
    object ob = new(STD_CONTAINER), ob2 = new(STD_CONTAINER);

    expect_function("handle_move", testOb);

    values += ({ sizeof(all_inventory(ob)) });
    results += ({ 0 });
    values += ({ testOb->handle_move(ob) });
    results += ({ 1 });
    values += ({ sizeof(all_inventory(ob)) });
    results += ({ 1 });
    values += ({ sizeof(all_inventory(ob2)) });
    results += ({ 0 });
    values += ({ testOb->handle_move(ob2) });
    results += ({ 1 });
    values += ({ sizeof(all_inventory(ob2)) });
    results += ({ 1 });
    values += ({ sizeof(all_inventory(ob)) });
    results += ({ 0 });

    expect_arrays_equal(values, results, "handle_move moved objects");
    destruct(ob);
    destruct(ob2);
}

void test_parser_applies () {
    int *values = ({}), *results = ({});

    expect_function("is_living", testOb);
    expect_function("inventory_visible", testOb);
    expect_function("inventory_accessible", testOb);

    values += ({ testOb->is_living() });
    results += ({ 0 });
    values += ({ testOb->inventory_visible() });
    results += ({ 0 });
    values += ({ testOb->inventory_accessible() });
    results += ({ 0 });

    expect_arrays_equal(values, results, "parser applies handled");
}