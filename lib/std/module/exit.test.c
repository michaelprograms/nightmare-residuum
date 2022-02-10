inherit M_TEST;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/exit.c");
}
void after_all_tests () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_exits", "test_handle_go", "test_exits_before_after" });
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
}

nosave private int checkBefore = 0, checkAfter = 0;

void test_exits_before_after () {
    mixed *values = ({}), *results = ({});
    object r1, r2, ob;

    checkBefore = 0;
    checkAfter = 0;

    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    ob = new(STD_NPC);

    r1->set_exit("east", file_name(r2), function (object ob, string dir) {
        checkBefore ++;
        return 1;
    }, function (object ob, string dir) {
        checkAfter ++;
    });
    r2->set_exit("west", file_name(r1));

    values += ({ ob->handle_move(r1) });
    results += ({ 1 });
    values += ({ r1->handle_go(ob, "walk", "east") });
    results += ({ 1 });
    values += ({ checkBefore });
    results += ({ 1 });
    values += ({ checkAfter });
    results += ({ 1 });

    r1->set_exit("east", file_name(r2), function (object ob, string dir) {
        return 0;
    });

    values += ({ ob->handle_move(r1) });
    results += ({ 1 });
    values += ({ r1->handle_go(ob, "walk", "east") });
    results += ({ 0 });

    expect_arrays_equal(values, results, "exits handled before and after functions");

    destruct(ob);
    destruct(r1);
    destruct(r2);
}

void test_handle_go () {
    mixed *values = ({}), *results = ({});
    object r1, r2, ob;

    expect_function("handle_go", testOb);

    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    ob = new(STD_NPC);

    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));
    r2->set_exit("east", "/invalid/path.c");

    values += ({ regexp(r1->query_exit("east"), "/std/room#[0-9]+") });
    results += ({ 1 });
    values += ({ regexp(r2->query_exit("west"), "/std/room#[0-9]+") });
    results += ({ 1 });
    values += ({ sizeof(r1->query_living_contents()) });
    results += ({ 0 });
    values += ({ sizeof(r2->query_living_contents()) });
    results += ({ 0 });
    values += ({ ob->handle_move(r1) });
    results += ({ 1 });
    values += ({ sizeof(r1->query_living_contents()) });
    results += ({ 1 });
    values += ({ sizeof(r2->query_living_contents()) });
    results += ({ 0 });
    values += ({ r1->handle_go(ob, "walk", "east") });
    results += ({ 1 });
    values += ({ file_name(environment(ob)) });
    results += ({ file_name(r2) });
    expect_arrays_equal(values, results, "handle_go moved object");

    values = ({});
    results = ({});
    ob->handle_move(r2);
    values += ({ r1->handle_go(ob, "walk", "east") });
    results += ({ 0 });
    values += ({ sizeof(r1->query_living_contents()) });
    results += ({ 0 });
    values += ({ sizeof(r2->query_living_contents()) });
    results += ({ 1 });
    expect_arrays_equal(values, results, "handle_go didn't move object it shouldn't");

    values = ({});
    results = ({});
    values += ({ r2->handle_go(ob, "walk", "east") });
    results += ({ 0 });
    expect_arrays_equal(values, results, "handle_go handled invalid path");

    if (ob) destruct(ob);
    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_query_defaults () {
    string *enterValues = ({}), *enterResults = ({});
    string *outValues = ({}), *outResults = ({});
    object r1, r2;

    expect_function("query_default_enter", testOb);
    expect_function("query_default_out", testOb);

    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);

    enterValues += ({ r1->query_default_enter() });
    enterResults += ({ 0 });
    outValues += ({ r2->query_default_out() });
    outResults += ({ 0 });

    r1->set_exits(([ "enter": file_name(r2) ]));
    r2->set_exits(([ "out": file_name(r1) ]));
    enterValues += ({ r1->query_default_enter() });
    enterResults += ({ "enter" });
    outValues += ({ r2->query_default_out() });
    outResults += ({ "out" });

    r1->set_exits(([ "enter east": file_name(r2) ]));
    r2->set_exits(([ "out west": file_name(r1) ]));
    enterValues += ({ r1->query_default_enter() });
    enterResults += ({ "enter east" });
    outValues += ({ r2->query_default_out() });
    outResults += ({ "out west" });

    r1->set_exits(([
        "enter east": file_name(r2),
        "enter west": file_name(r2),
    ]));
    r2->set_exits(([
        "out west": file_name(r1),
        "out east": file_name(r1),
    ]));
    enterValues += ({ r1->query_default_enter() });
    enterResults += ({ 0 });
    outValues += ({ r2->query_default_out() });
    outResults += ({ 0 });

    r1->remove_exit("enter east");
    r2->remove_exit("out west");
    enterValues += ({ r1->query_default_enter() });
    enterResults += ({ "enter west" });
    outValues += ({ r2->query_default_out() });
    outResults += ({ "out east" });

    expect_arrays_equal(enterValues, enterResults, "query_default_enter behaved");
    expect_arrays_equal(outValues, outResults, "query_default_out behaved");

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

// @TODO bad arguments