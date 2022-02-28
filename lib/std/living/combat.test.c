inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/living/combat.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_hostiles () {
    object ob1 = new(STD_LIVING), ob2 = new(STD_LIVING);
    int *values = ({}), *results = ({});

    expect_function("add_hostile", testOb);
    expect_function("remove_hostile", testOb);
    expect_function("query_hostiles", testOb);

    // test adding and removing
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 0 }); // no hostiles
    values += ({ testOb->add_hostile(ob1) });
    results += ({ 1 }); // 1st hostile added
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 1 }); // 1 hostile
    values += ({ testOb->add_hostile(ob1) });
    results += ({ 0 }); // can't add same hostile
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 1 }); // still 1 hostile
    values += ({ testOb->add_hostile(ob2) });
    results += ({ 1 }); // 2nd hostile added
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 2 }); // 2 hostiles
    values += ({ testOb->remove_hostile(ob1) });
    results += ({ 1 }); // 1st hostile removed
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 1 }); // 1 hostile
    values += ({ testOb->remove_hostile(ob1) });
    results += ({ 0 }); // can't remove same hostile
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 1 }); // still 1 hostile
    values += ({ testOb->remove_hostile(ob2) });
    results += ({ 1 }); // 2nd hostile removed
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 0 }); // 0 hostiles

    // test filtering undefined
    values += ({ testOb->add_hostile(ob1) });
    results += ({ 1 }); // 1st hostile added
    values += ({ testOb->add_hostile(ob2) });
    results += ({ 1 }); // 2nd hostile added
    values += ({ ob2->handle_remove() });
    results += ({ 1 }); // 2nd hostile destructed
    values += ({ sizeof(testOb->query_hostiles()) });
    results += ({ 1 }); // only 1 hostile

    expect_arrays_equal(values, results, "handled adding and removing hostiles");

    if (ob1) ob1->handle_remove();
    if (ob2) ob2->handle_remove();
}