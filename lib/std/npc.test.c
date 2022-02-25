inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/npc.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_npc () {
    int *values = ({}), *results = ({});

    expect_function("is_npc", testOb);

    values += ({ testOb->is_living() });
    results += ({ 1 });
    values += ({ testOb->is_npc() });
    results += ({ 1 });
    values += ({ testOb->is_monster() });
    results += ({ UNDEFINED });
    values += ({ testOb->is_character() });
    results += ({ UNDEFINED });

    expect_arrays_equal(values, results, "is_npc handled");
}