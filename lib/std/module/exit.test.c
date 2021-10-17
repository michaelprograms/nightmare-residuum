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
    mixed *values = ({}), *results = ({});

    expect_function("query_exits", testOb);
    expect_function("query_exit", testOb);
    expect_function("query_exit_directions", testOb);
    expect_function("query_exit_dirs", testOb);
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
    values += ({ testOb->query_exit_dirs() });
    results += ({ ({ "n" }) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({ ([ "room": "/northroom.c" ]) }) });

    testOb->set_exit("south", "/southroom.c");
    values += ({ testOb->query_exits() });
    results += ({ ([ "south": ([ "room": "/southroom.c"]), "north": ([ "room": "/northroom.c" ]) ]) });
    values += ({ testOb->query_exit_directions() });
    results += ({ ({ "south", "north" }) });
    values += ({ testOb->query_exit_dirs() });
    results += ({ ({ "s", "n" }) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({ ([ "room": "/southroom.c" ]), ([ "room": "/northroom.c" ]) }) });

    testOb->remove_exit("north");
    values += ({ testOb->query_exits() });
    results += ({ ([ "south": ([ "room": "/southroom.c" ]) ]) });
    values += ({ testOb->query_exit_directions() });
    results += ({ ({ "south" }) });
    values += ({ testOb->query_exit_dirs() });
    results += ({ ({ "s" }) });
    values += ({ testOb->query_exit_destinations() });
    results += ({ ({ ([ "room": "/southroom.c" ]) }) });

    testOb->set_exit("north", "/northroom.c");
    testOb->set_exit("northeast", "/northeastroom.c");
    testOb->set_exit("northwest", "/northwestroom.c");
    testOb->set_exit("east", "/eastroom.c");
    testOb->set_exit("west", "/westroom.c");
    testOb->set_exit("southeast", "/southeastroom.c");
    testOb->set_exit("southwest", "/southwestroom.c");
    values += ({ testOb->query_exit_directions() });
    results += ({ ({ "north", "south", "east", "southeast", "northeast", "west", "southwest", "northwest" }) });
    values += ({ testOb->query_exit_dirs() });
    results += ({ ({ "n", "s", "e", "se", "ne", "w", "sw", "nw" }) });

    expect_arrays_equal(values, results, "exits handled adding and removing");

    // @TODO test pre/post
}

void test_handle_go () {
    mixed *values = ({}), *results = ({});
    object r1, r2, ob;

    expect_function("handle_go", testOb);

    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    ob = new(STD_OBJECT);

    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    values += ({ regexp(r1->query_exit("east"), "/std/room#[0-9]+") });
    results += ({ 1 });
    values += ({ regexp(r2->query_exit("west"), "/std/room#[0-9]+") });
    results += ({ 1 });
    values += ({ sizeof(r1->query_nonliving_contents()) });
    results += ({ 0 });
    values += ({ sizeof(r2->query_nonliving_contents()) });
    results += ({ 0 });
    values += ({ ob->handle_move(r1) });
    results += ({ 1 });
    values += ({ sizeof(r1->query_nonliving_contents()) });
    results += ({ 1 });
    values += ({ sizeof(r2->query_nonliving_contents()) });
    results += ({ 0 });
    values += ({ r1->handle_go(ob, "east") });
    results += ({ 1 });
    values += ({ sizeof(r1->query_nonliving_contents()) });
    results += ({ 0 });
    values += ({ sizeof(r2->query_nonliving_contents()) });
    results += ({ 1 });

    expect_arrays_equal(values, results, "handle_go moved object");

    destruct(ob);
    destruct(r1);
    destruct(r2);
}