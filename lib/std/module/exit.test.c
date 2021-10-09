inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/exit.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}

void test_exits () {
    int *values = ({}), *results = ({});

    expect_function("query_exits", testOb);
    expect_function("query_exit", testOb);
    expect_function("query_exit_directions", testOb);
    expect_function("query_exit_destinations", testOb);
    expect_function("set_exit", testOb);
    expect_function("set_exits", testOb);
    expect_function("remove_exit", testOb);

    values += ({ testOb->query_exits() });
    results += ({ ([]) });
    values += ({ testOb->query_exit_directions() });
    results += ({ ({}) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({}) });

    testOb->set_exit("north", "/northroom.c");
    values += ({ testOb->query_exits() });
    results += ({ ([ "north": ([ "room": "/northroom.c" ]) ]) });
    values += ({ testOb->query_exit_directions() });
    results += ({ ({ "north" }) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({ ([ "room": "/northroom.c" ]) }) });

    testOb->set_exit("south", "/southroom.c");
    values += ({ testOb->query_exits() });
    results += ({ ([ "south": ([ "room": "/southroom.c"]), "north": ([ "room": "/northroom.c" ]) ]) });
    values += ({ testOb->query_exit_directions() });
    results += ({ ({ "south", "north" }) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({ ([ "room": "/southroom.c" ]), ([ "room": "/northroom.c" ]) }) });

    testOb->remove_exit("north");
    values += ({ testOb->query_exits() });
    results += ({ ([ "south": ([ "room": "/southroom.c" ]) ]) });
    values += ({ testOb->query_exit_directions() });
    results += ({ ({ "south" }) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({ ([ "room": "/southroom.c" ]) }) });

    expect_arrays_equal(values, results, "exits handled query_exit/set_exit/remove_exit");

    // @TODO test pre/post
}