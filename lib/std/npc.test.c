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

void test_set_level () {
    int *values = ({}), *results = ({});

    expect_function("set_level", testOb);

    values += ({ testOb->query_level() });
    results += ({ 0 });
    values += ({ testOb->query_stat("strength") });
    results += ({ 0 });
    values += ({ testOb->query_stat("endurance") });
    results += ({ 0 });
    values += ({ testOb->query_skill("melee attack") });
    results += ({ 0 });
    values += ({ testOb->query_skill("melee defense") });
    results += ({ 0 });

    testOb->set_level(10);
    values += ({ testOb->query_level() });
    results += ({ 10 });
    values += ({ testOb->query_stat("strength") > 0 });
    results += ({ 1 });
    values += ({ testOb->query_stat("endurance") > 0 });
    results += ({ 1 });
    values += ({ testOb->query_skill("melee attack") > 0 });
    results += ({ 1 });
    values += ({ testOb->query_skill("melee defense") > 0 });
    results += ({ 1 });

    expect_arrays_equal(values, results, "set_level adjusts stats and skills");
}