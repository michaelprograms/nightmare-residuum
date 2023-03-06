inherit M_TEST;
inherit M_MOVE;

private nosave object testOb;
void before_each_test () {
    if (objectp(testOb)) destruct(testOb);
    testOb = clone_object("/std/module/exit.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
string *test_order () {
    return ({ "test_exits", "test_handle_go", "test_exits_before_after" });
}

void test_exits () {
    expect_function("query_exits", testOb);
    expect_function("query_exit", testOb);
    expect_function("query_exit_directions", testOb);
    expect_function("query_exit_dirs", testOb);
    expect_function("query_exit_destinations", testOb);
    expect_function("set_exit", testOb);
    expect_function("set_exits", testOb);
    expect_function("remove_exit", testOb);

    expect("exits handles adding and removing", (: ({
        assert(testOb->query_exits(), "==", ([])),
        assert(testOb->query_exit_directions(), "==", ({})),
        assert(testOb->query_exit_destinations(), "==", ({})),

        testOb->set_exit("north", "/northroom.c"),
        assert(testOb->query_exits(), "==", ([ "north": ([ "room": "/northroom.c" ]) ])),
        assert(testOb->query_exit_directions(), "==", ({ "north" })),
        assert(testOb->query_exit_dirs(), "==", ({ "n" })),
        assert(testOb->query_exit_destinations(), "==", ({ ([ "room": "/northroom.c" ]) })),

        testOb->set_exit("south", "/southroom.c"),
        assert(testOb->query_exits(), "==", ([ "south": ([ "room": "/southroom.c"]), "north": ([ "room": "/northroom.c" ]) ])),
        assert(testOb->query_exit_directions(), "==", ({ "south", "north" })),
        assert(testOb->query_exit_dirs(), "==", ({ "s", "n" })),
        assert(testOb->query_exit_destinations(), "==", ({ ([ "room": "/southroom.c" ]), ([ "room": "/northroom.c" ]) })),

        testOb->remove_exit("north"),
        assert(testOb->query_exits(), "==", ([ "south": ([ "room": "/southroom.c" ]) ])),
        assert(testOb->query_exit_directions(), "==", ({ "south" })),
        assert(testOb->query_exit_dirs(), "==", ({ "s" })),
        assert(testOb->query_exit_destinations(), "==", ({ ([ "room": "/southroom.c" ]) })),

        testOb->set_exit("north", "/northroom.c"),
        testOb->set_exit("northeast", "/northeastroom.c"),
        testOb->set_exit("northwest", "/northwestroom.c"),
        testOb->set_exit("east", "/eastroom.c"),
        testOb->set_exit("west", "/westroom.c"),
        testOb->set_exit("southeast", "/southeastroom.c"),
        testOb->set_exit("southwest", "/southwestroom.c"),
        assert(testOb->query_exit_directions(), "==", ({ "north", "south", "east", "southeast", "northeast", "west", "southwest", "northwest" })),
        assert(testOb->query_exit_dirs(), "==", ({ "n", "s", "e", "se", "ne", "w", "sw", "nw" })),
    }) :));
}

nosave private int checkBefore = 0, checkAfter = 0;
nosave private object r1, r2, ob;

void test_exits_before_after () {
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

    expect("exits handles before and after functions", (: ({
        assert(ob->handle_move(r1), "==", 1),
        assert(r1->handle_go(ob, "walk", "east"), "==", 1),
        assert(checkBefore, "==", 1),
        assert(checkAfter, "==", 1),

        r1->set_exit("east", file_name(r2), function (object ob, string dir) {
            return 0;
        }),

        assert(ob->handle_move(r1), "==", 1),
        assert(r1->handle_go(ob, "walk", "east"), "==", 0),
    }) :));

    destruct(ob);
    destruct(r1);
    destruct(r2);
}

void test_handle_go () {
    expect_function("handle_go", testOb);

    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    ob = new(STD_NPC);

    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));
    r2->set_exit("east", "/invalid/path.c");

    expect("handle_go moved object", (: ({
        assert(regexp(r1->query_exit("east"), "/std/room#[0-9]+"), "==", 1),
        assert(regexp(r2->query_exit("west"), "/std/room#[0-9]+"), "==", 1),
        assert(sizeof(r1->query_living_contents()), "==", 0),
        assert(sizeof(r2->query_living_contents()), "==", 0),
        assert(ob->handle_move(r1), "==", 1),
        assert(sizeof(r1->query_living_contents()), "==", 1),
        assert(sizeof(r2->query_living_contents()), "==", 0),

        assert(r1->handle_go(ob, "walk", "east"), "==", 1),
        assert(file_name(environment(ob)), "==", file_name(r2)),
    }) :));

    expect("handle_go doesn't move objects it shouldn't", (: ({
        ob->handle_move(r2),
        assert(r1->handle_go(ob, "walk", "east"), "==", 0),
        assert(sizeof(r1->query_living_contents()), "==", 0),
        assert(sizeof(r2->query_living_contents()), "==", 1),
    }) :));

    expect("handle_go handles invalid path", (: ({
        assert(r2->handle_go(ob, "walk", "east"), "==", 0),
    }) :));

    if (ob) destruct(ob);
    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_query_defaults () {
    expect_function("query_default_enter", testOb);
    expect_function("query_default_out", testOb);

    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);

    expect("handles default enter and out", (: ({
        assert(r1->query_default_enter(), "==", 0),
        assert(r2->query_default_out(), "==", 0),

        r1->set_exits(([ "enter": file_name(r2) ])),
        r2->set_exits(([ "out": file_name(r1) ])),
        assert(r1->query_default_enter(), "==", "enter"),
        assert(r2->query_default_out(), "==", "out"),

        r1->set_exits(([ "enter east": file_name(r2) ])),
        r2->set_exits(([ "out west": file_name(r1) ])),
        assert(r1->query_default_enter(), "==", "enter east"),
        assert(r2->query_default_out(), "==", "out west"),

        r1->set_exits(([
            "enter east": file_name(r2),
            "enter west": file_name(r2),
        ])),
        r2->set_exits(([
            "out west": file_name(r1),
            "out east": file_name(r1),
        ])),
        assert(r1->query_default_enter(), "==", 0),
        assert(r2->query_default_out(), "==", 0),

        r1->remove_exit("enter east"),
        r2->remove_exit("out west"),
        assert(r1->query_default_enter(), "==", "enter west"),
        assert(r2->query_default_out(), "==", "out east"),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

// catch reverse override for test_exit_reverse_override
nosave private string __Reverse;
varargs int handle_go (mixed dest, string verb, string dir, string reverse) {
    __Reverse = reverse;
}

void test_exit_reverse_override () {
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);

    __Reverse = 0;
    expect("set exit uses reverse override", (: ({
        // setup room test ob
        r1->set_exit("east", file_name(r2), 0, 0, "reverse"),
        r1->set_exit("west", file_name(r2), 0, 0, "override"),
        // move test to room test ob
        assert(this_object()->handle_move(r1), "==", 1),

        // verify no reverse overrides seen yet
        assert(__Reverse, "==", 0),

        // try first override
        assert(r1->handle_go(this_object(), "method", "east"), "==", 1),
        assert(__Reverse, "==", "reverse"),

        // try second override
        assert(r1->handle_go(this_object(), "method", "west"), "==", 1),
        assert(__Reverse, "==", "override"),

        assert(this_object()->handle_move("/domain/Nowhere/room/void.c"), "==", 1),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_doors () {
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);

    expect_function("query_doors", testOb);
    expect_function("query_open", testOb);
    expect_function("query_locked", testOb);
    // expect_function("set_open", testOb);
    // expect_function("set_locked", testOb);
    // expect_function("query_door_dir", testOb);
    // expect_function("handle_open", testOb);
    // expect_function("handle_close", testOb);
    // expect_function("handle_lock", testOb);
    // expect_function("handle_unlock", testOb);

    expect("doors behave", (: ({
        // no doors are setup
        assert(r1->query_doors(), "==", ({ })),
        assert(r2->query_doors(), "==", ({ })),
        assert(r1->query_open("door"), "==", -1),
        assert(r2->query_open("door"), "==", -1),
        assert(r1->query_locked("door"), "==", -1),
        assert(r2->query_locked("door"), "==", -1),

        // setup door
        r1->set_exit("east", file_name(r2), 0, 0, 0, "door", 0, 0),
        r2->set_exit("west", file_name(r1), 0, 0, 0, "door", 0, 0),

        // doors are setup
        assert(r1->query_doors(), "==", ({ "door", })),
        assert(r2->query_doors(), "==", ({ "door", })),
        assert(r1->query_doors(1), "==", ({ "east", "door", })),
        assert(r2->query_doors(1), "==", ({ "west", "door", })),
        assert(r1->query_open("door"), "==", 0),
        assert(r2->query_open("door"), "==", 0),
        assert(r1->query_locked("door"), "==", 0),
        assert(r2->query_locked("door"), "==", 0),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}