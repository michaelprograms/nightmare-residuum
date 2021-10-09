inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/container");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_receive () {
    object ob = new(STD_OBJECT);
    int *values = ({}), *results = ({});

    expect_function("can_receive", testOb);
    expect_function("can_release", testOb);
    expect_function("handle_receive_object", testOb);
    expect_function("handle_release_object", testOb);

    values += ({ testOb->can_receive(ob) });
    results += ({ 1 });

    values += ({ testOb->can_release(ob) });
    results += ({ 0 });

    values += ({ testOb->handle_receive_object(ob) });
    results += ({ 1 });

    values += ({ ob->handle_move(testOb) });
    results += ({ 1 });

    values += ({ testOb->can_receive(ob) });
    results += ({ 0 });

    values += ({ testOb->can_release(ob) });
    results += ({ 1 });

    values += ({ testOb->handle_release_object(ob) });
    results += ({ 1 });

    expect_arrays_equal(values, results, "receive and release behave");
}