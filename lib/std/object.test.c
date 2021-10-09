inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/object.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_handle_remove () {
    int *values = ({});

    expect_function("handle_remove", testOb);
    expect_true(member_array("internal_remove", functions(testOb, 0)) > -1 && !function_exists("internal_remove", testOb), "internal_remove is protected");

    values += ({ objectp(testOb) });
    values += ({ testOb->handle_remove() });
    values += ({ objectp(testOb) });
    expect_arrays_equal(values, ({
        1,  // objectp
        1,  // handle_remove
        0,  // objectp
    }), "handle_remove behaves");
}

void test_lifecycle_functions () {
    // @TODO test these better

    expect_function("create", testOb);

    expect_function("heart_beat", testOb);

    expect_function("id", testOb);
    // Tests for id handled by /std/object/id.test.c

    expect_function("move_or_destruct", testOb);

    expect_function("reset", testOb);

    expect_function("virtual_start", testOb);
}

void test_move () {
    object ob = new(STD_CONTAINER);

    expect_true(testOb->handle_move(ob) == 1, "handle_move returned 1");
    expect_true(sizeof(all_inventory(ob)) == 1, "handle_move put object in inventory");
}