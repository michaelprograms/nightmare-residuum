inherit M_TEST;
inherit M_MOVE;

/**
 * @var {"/std/module/exit"} testOb
 */

string *test_order () {
    return ({
        "test_exits",
        "test_climbs",
        "test_handle_go",
        "test_exits_before_after",
    });
}

void test_exits () {
    function fn = function() {};

    expect("exits are addable, queryable, and removable", (: ({
        assert_equal(testOb->query_exit("invalid"), 0),

        assert_equal(testOb->query_exits(), ([ ])),
        assert_equal(testOb->query_exit_directions(), ({ })),
        assert_equal(testOb->query_exit_destinations(), ({ })),

        testOb->set_exit("north", "/northroom.c"),
        assert_equal(testOb->query_exits(), ([ "north": ([ "room": "/northroom.c" ]) ])),
        assert_equal(testOb->query_exit_directions(), ({ "north" })),
        assert_equal(testOb->query_exit_dirs(), ({ "n" })),
        assert_equal(testOb->query_exit_destinations(), ({ ([ "room": "/northroom.c" ]) })),
        assert_equal(testOb->query_exit("north"), "/northroom.c"),

        testOb->set_exit("enter west", "/westroom.c"),
        assert_equal(testOb->query_exit("west"), "/westroom.c"),
        assert_equal(testOb->query_default_enter(), "enter west"),
        testOb->set_exit("out east", "/eastroom.c"),
        assert_equal(testOb->query_exit("east"), "/eastroom.c"),
        assert_equal(testOb->query_default_out(), "out east"),
        testOb->remove_exit("enter west"),
        testOb->remove_exit("out east"),

        testOb->set_exit("south", "/southroom.c"),
        assert_equal(testOb->query_exits(), ([ "south": ([ "room": "/southroom.c"]), "north": ([ "room": "/northroom.c" ]) ])),
        assert_equal(testOb->query_exit_directions(), ({ "south", "north" })),
        assert_equal(testOb->query_exit_dirs(), ({ "s", "n" })),
        assert_equal(testOb->query_exit_destinations(), ({ ([ "room": "/southroom.c" ]), ([ "room": "/northroom.c" ]) })),

        testOb->remove_exit("north"),
        assert_equal(testOb->query_exits(), ([ "south": ([ "room": "/southroom.c" ]) ])),
        assert_equal(testOb->query_exit_directions(), ({ "south" })),
        assert_equal(testOb->query_exit_dirs(), ({ "s" })),
        assert_equal(testOb->query_exit_destinations(), ({ ([ "room": "/southroom.c" ]) })),

        // add more exits
        testOb->set_exit("north", "/northroom.c"),
        testOb->set_exit("northeast", "/northeastroom.c"),
        testOb->set_exit("northwest", "/northwestroom.c"),
        testOb->set_exit("east", "/eastroom.c"),
        testOb->set_exit("west", "/westroom.c"),
        testOb->set_exit("southeast", "/southeastroom.c"),
        testOb->set_exit("southwest", "/southwestroom.c"),
        assert_equal(testOb->query_exit_directions(), ({ "north", "south", "east", "southeast", "northeast", "west", "southwest", "northwest" })),
        assert_equal(testOb->query_exit_dirs(), ({ "n", "s", "e", "se", "ne", "w", "sw", "nw" })),

        // overrides existing exits
        testOb->set_exits(([
            "north": "/northroom.c",
            "east": "/eastroom.c",
            "west": "/westroom.c",
            "south": "/southroom.c",
        ])),
        assert_equal(testOb->query_exit_directions(), ({ "north", "south", "east", "west" })),
        assert_equal(testOb->query_exit_dirs(), ({ "n", "s", "e", "w" })),
        assert_equal(testOb->query_exits(), ([ "east": ([ "room": "/eastroom.c" ]), "north": ([ "room": "/northroom.c" ]), "south": ([ "room": "/southroom.c" ]), "west": ([ "room": "/westroom.c" ]) ])),
        // array dir and dests
        testOb->set_exits(([
            ({ "northeast" }): ({ "/northeastroom.c", 0, 0, "ne_reverse" }),
            ({ "southeast" }): "/southeastroom.c",
            "southwest": ({ "/southwestroom.c", 0, 0, "sw_reverse" }),
            "northwest": "/northwestroom.c",
        ])),
        assert_equal(testOb->query_exits(), ([ "northeast": ([ "reverse": "ne_reverse", "room": "/northeastroom.c" ]), "northwest": ([ "room": "/northwestroom.c" ]), "southeast": ([ "room": "/southeastroom.c" ]), "southwest": ([ "reverse": "sw_reverse", "room": "/southwestroom.c" ]) ])),

        // test all options
        testOb->set_exit("north", "/northroom.c", $(fn), $(fn), "reverse", "door", "key", UNDEFINED),
        assert_equal(testOb->query_exits()["north"], ([ "after": $(fn), "before": $(fn), "door": "door", "key": "key", "locked": 1, "open": 0, "reverse": "reverse", "room": "/northroom.c" ])),
        testOb->set_exit("north", "/northroom.c", $(fn), $(fn), "reverse", "door", "key", 0),
        assert_equal(testOb->query_exits()["north"], ([ "after": $(fn), "before": $(fn), "door": "door", "key": "key", "locked": 0, "open": 0, "reverse": "reverse", "room": "/northroom.c" ])),
        testOb->set_exit("north", "/northroom.c", $(fn), $(fn), "reverse", "door", "key", 1),
        assert_equal(testOb->query_exits()["north"], ([ "after": $(fn), "before": $(fn), "door": "door", "key": "key", "locked": 1, "open": 0, "reverse": "reverse", "room": "/northroom.c" ])),
    }) :));

    expect("exits are able to set hidden flag", (: ({
        assert_equal(testOb->query_hidden_exits(), 0),
        testOb->set_hidden_exits(1),
        assert_equal(testOb->query_hidden_exits(), 1),
        testOb->set_hidden_exits(0),
        assert_equal(testOb->query_hidden_exits(), 0),
    }) :));

    expect("exits handles bad inputs", (: ({
        assert_catch((: testOb->set_exit(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to exit->set_exit\n"),
        assert_catch((: testOb->set_exit("dir", UNDEFINED) :), "*Bad argument 2 to exit->set_exit\n"),

        assert_catch((: testOb->remove_exit(UNDEFINED) :), "*Bad argument 1 to exit->remove_exit\n"),
    }) :));
}

// Necessary for handle_open, handle_close, handle_lock, handle_unlock
string query_cap_name () { return "Test"; }

void test_climbs () {
    function fn = function() {};

    expect("climbs are addable and queryable", (: ({
        assert_equal(testOb->query_climbs(), ([ ])),
        assert_equal(testOb->query_climb_directions(), ({ })),
        assert_equal(testOb->query_climb_destinations(), ({ })),
        assert_equal(testOb->query_climb("up"), 0),

        testOb->set_climb("up", "/uproom.c"),
        assert_equal(testOb->query_climbs(), ([ "up": ([ "room": "/uproom.c" ]) ])),
        assert_equal(testOb->query_climb_directions(), ({ "up" })),
        assert_equal(testOb->query_climb_destinations(), ({ ([ "room": "/uproom.c" ]) })),
        assert_equal(testOb->query_climb("up"), "/uproom.c"),

        testOb->set_climb("down", "/downroom.c"),
        assert_equal(testOb->query_climbs(), ([ "down": ([ "room": "/downroom.c"]), "up": ([ "room": "/uproom.c" ]) ])),
        assert_equal(testOb->query_climb_directions(), ({ "down", "up" })),
        assert_equal(testOb->query_climb_destinations(), ({ ([ "room": "/downroom.c" ]), ([ "room": "/uproom.c" ]) })),

        testOb->remove_climb("down"),
        assert_equal(testOb->query_climbs(), ([ "up": ([ "room": "/uproom.c" ]) ])),
        assert_equal(testOb->query_climb_directions(), ({ "up" })),
        assert_equal(testOb->query_climb_destinations(), ({ ([ "room": "/uproom.c" ]) })),

        // test all options
        testOb->set_climb("up", "/uproom.c", $(fn), $(fn), "reverse"),
        assert_equal(testOb->query_climbs()["up"], ([ "after": $(fn), "before": $(fn), "reverse": "reverse", "room": "/uproom.c" ])),

        // override climbs
        testOb->set_climbs(([
            "up": "/uproom2.c",
            "down": "/downroom2.c",
            "something": "/somethingroom.c",
        ])),
        assert_equal(testOb->query_climbs(), ([ "down": ([ "room": "/downroom2.c"]), "something": ([ "room": "/somethingroom.c" ]), "up": ([ "room": "/uproom2.c", ]) ])),
        assert_equal(testOb->query_climb_directions(), ({ "something", "down", "up", })),
        assert_equal(testOb->query_climb_destinations(), ({ ([ "room": "/somethingroom.c" ]), ([ "room": "/downroom2.c" ]), ([ "room": "/uproom2.c" ]), })),
        // array dir and dests
        testOb->set_climbs(([
            ({ "up", }): ({ "/uproom.c", 0, 0, "down" }),
            ({ "up2" }): "/up2room.c",
            "down": ({ "/downroom.c", 0, 0, "up" }),
            "down2": "/down2room.c",
        ])),
        assert_equal(testOb->query_climbs(), ([ "up": ([ "reverse": "down", "room": "/uproom.c" ]), "down2": ([ "room": "/down2room.c" ]), "up2": ([ "room": "/up2room.c" ]), "down": ([ "reverse": "up", "room": "/downroom.c" ]) ])),
    }) :));

    expect("climbs handles bad inputs", (: ({
        assert_catch((: testOb->set_climb(UNDEFINED, UNDEFINED) :), "*Bad argument 1 to exit->set_climb\n"),
        assert_catch((: testOb->set_climb("dir", UNDEFINED) :), "*Bad argument 2 to exit->set_climb\n"),

        assert_catch((: testOb->remove_climb(UNDEFINED) :), "*Bad argument 1 to exit->remove_climb\n"),
    }) :));
}

nosave private int checkBefore = 0, checkAfter = 0;
/** @type {STD_ROOM} r1 */
nosave private object r1;
/** @type {STD_ROOM} r1 */
nosave private object r2;
/** @type {STD_NPC} ob */
nosave private object ob;

// TODO: Test this through testOb and mocks
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
        assert_equal(ob->handle_move(r1), 1),
        assert_equal(r1->handle_go(ob, "walk", "east"), 1),
        assert_equal(checkBefore, 1),
        assert_equal(checkAfter, 1),

        r1->set_exit("east", file_name(r2), function (object ob, string dir) {
            return 0;
        }),

        assert_equal(ob->handle_move(r1), 1),
        assert_equal(r1->handle_go(ob, "walk", "east"), 0),
    }) :));

    destruct(ob);
    destruct(r1);
    destruct(r2);
}

void test_handle_go () {
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    ob = new(STD_NPC);

    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));
    r2->set_exit("east", "/invalid/path.c");

    expect("handle_go moved object", (: ({
        assert_equal(regexp(r1->query_exit("east"), "/std/room#[0-9]+"), 1),
        assert_equal(regexp(r2->query_exit("west"), "/std/room#[0-9]+"), 1),
        assert_equal(sizeof(r1->query_living_contents()), 0),
        assert_equal(sizeof(r2->query_living_contents()), 0),
        assert_equal(ob->handle_move(r1), 1),
        assert_equal(sizeof(r1->query_living_contents()), 1),
        assert_equal(sizeof(r2->query_living_contents()), 0),

        assert_equal(r1->handle_go(ob, "walk", "east"), 1),
        assert_equal(file_name(environment(ob)), file_name(r2)),
    }) :));

    expect("handle_go doesn't move objects it shouldn't", (: ({
        ob->handle_move(r2),
        assert_equal(r1->handle_go(ob, "walk", "east"), 0),
        assert_equal(sizeof(r1->query_living_contents()), 0),
        assert_equal(sizeof(r2->query_living_contents()), 1),
    }) :));

    expect("handle_go handles invalid path", (: ({
        assert_equal(r2->handle_go(ob, "walk", "east"), 0),
    }) :));

    if (ob) destruct(ob);
    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_handle_climb () {
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    ob = new(STD_NPC);

    r1->set_climb("up", file_name(r2));
    r2->set_climb("down", file_name(r1));
    r2->set_climb("bad", "/invalid/path.c");

    expect("handle_climb moved object", (: ({
        assert_equal(regexp(r1->query_climb("up"), "/std/room#[0-9]+"), 1),
        assert_equal(regexp(r2->query_climb("down"), "/std/room#[0-9]+"), 1),
        assert_equal(sizeof(r1->query_living_contents()), 0),
        assert_equal(sizeof(r2->query_living_contents()), 0),
        assert_equal(ob->handle_move(r1), 1),
        assert_equal(sizeof(r1->query_living_contents()), 1),
        assert_equal(sizeof(r2->query_living_contents()), 0),

        assert_equal(r1->handle_climb(ob, "climb", "up"), 1),
        assert_equal(file_name(environment(ob)), file_name(r2)),
    }) :));

    expect("handle_climb doesn't move objects it shouldn't", (: ({
        ob->handle_move(r2),
        assert_equal(r1->handle_climb(ob, "climb", "up"), 0),
        assert_equal(sizeof(r1->query_living_contents()), 0),
        assert_equal(sizeof(r2->query_living_contents()), 1),
    }) :));

    expect("handle_climb handles invalid path", (: ({
        assert_equal(r2->handle_climb(ob, "climb", "bad"), 0),
    }) :));

    if (ob) destruct(ob);
    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_query_defaults () {
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);

    expect("handles default enter and out", (: ({
        assert_equal(r1->query_default_enter(), 0),
        assert_equal(r2->query_default_out(), 0),

        r1->set_exits(([ "enter": file_name(r2) ])),
        r2->set_exits(([ "out": file_name(r1) ])),
        assert_equal(r1->query_default_enter(), "enter"),
        assert_equal(r2->query_default_out(), "out"),

        r1->set_exits(([ "enter east": file_name(r2) ])),
        r2->set_exits(([ "out west": file_name(r1) ])),
        assert_equal(r1->query_default_enter(), "enter east"),
        assert_equal(r2->query_default_out(), "out west"),

        r1->set_exits(([
            "enter east": file_name(r2),
            "enter west": file_name(r2),
        ])),
        r2->set_exits(([
            "out west": file_name(r1),
            "out east": file_name(r1),
        ])),
        assert_equal(r1->query_default_enter(), 0),
        assert_equal(r2->query_default_out(), 0),

        r1->remove_exit("enter east"),
        r2->remove_exit("out west"),
        assert_equal(r1->query_default_enter(), "enter west"),
        assert_equal(r2->query_default_out(), "out east"),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

// catch reverse override for test_exit_reverse_override
nosave private string __Reverse;
varargs int handle_go (mixed dest, string verb, string dir, string reverse) {
    __Reverse = reverse;
    return 1;
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
        assert_equal(this_object()->handle_move(r1), 1),

        // verify no reverse overrides seen yet
        assert_equal(__Reverse, 0),

        // try first override
        assert_equal(r1->handle_go(this_object(), "method", "east"), 1),
        assert_equal(__Reverse, "reverse"),

        // try second override
        assert_equal(r1->handle_go(this_object(), "method", "west"), 1),
        assert_equal(__Reverse, "override"),

        assert_equal(this_object()->handle_move("/domain/Nowhere/room/void.c"), 1),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_doors () {
    r1 = new(STD_ROOM);

    expect("regular doors behave", (: ({
        // no doors are setup
        assert_equal(r1->query_doors(), ({ })),
        assert_equal(testOb->query_doors(), ({ })),
        assert_equal(r1->query_open("door"), -1),
        assert_equal(testOb->query_open("door"), -1),
        assert_equal(r1->query_open("east"), -1),
        assert_equal(testOb->query_open("west"), -1),
        assert_equal(r1->query_locked("door"), -1),
        assert_equal(testOb->query_locked("door"), -1),
        assert_equal(r1->query_locked("east"), -1),
        assert_equal(testOb->query_locked("west"), -1),
        assert_equal(r1->query_dir_door("east"), 0),
        assert_equal(testOb->query_dir_door("west"), 0),

        // setup door without key
        r1->set_exit("east", file_name(testOb), 0, 0, 0, "door", 0, 0),
        testOb->set_exit("west", file_name(r1), 0, 0, 0, "door", 0, 0),
        // doors are setup
        assert_equal(r1->query_doors(), ({ "door", })),
        assert_equal(testOb->query_doors(), ({ "door", })),
        assert_equal(r1->query_doors(1), ({ "east", "door", })),
        assert_equal(testOb->query_doors(1), ({ "west", "door", })),
        assert_equal(r1->query_dir_door("east"), "door"),
        assert_equal(testOb->query_dir_door("west"), "door"),
        assert_equal(r1->query_door_dir("door"), "east"),
        assert_equal(testOb->query_door_dir("door"), "west"),
        assert_equal(r1->query_open("door"), 0),
        assert_equal(r1->query_open("east"), 0),
        assert_equal(testOb->query_open("door"), 0),
        assert_equal(testOb->query_open("west"), 0),
        assert_equal(r1->query_locked("door"), 0),
        assert_equal(testOb->query_locked("door"), 0),
        assert_equal(r1->query_locked("east"), 0),
        assert_equal(testOb->query_locked("west"), 0),

        assert_equal(r1->query_door_dir("non-existent"), 0),
        assert_equal(testOb->query_door_dir("non-existent"), 0),
    }) :));
    expect("lockable doors behave", (: ({
        // setup locked door with key
        r1->set_exit("east", file_name(testOb), 0, 0, 0, "door", "test key", 1),
        testOb->set_exit("west", file_name(r1), 0, 0, 0, "door", "test key", 1),
        // doors are setup
        assert_equal(r1->query_open("door"), 0),
        assert_equal(testOb->query_open("door"), 0),
        assert_equal(r1->query_locked("door"), 1),
        assert_equal(testOb->query_locked("door"), 1),

        // setup unlocked door with key
        r1->set_exit("east", file_name(testOb), 0, 0, 0, "door", "test key", 0),
        testOb->set_exit("west", file_name(r1), 0, 0, 0, "door", 0, 0),
        // doors are setup and closed
        assert_equal(r1->query_open("door"), 0),
        assert_equal(testOb->query_open("door"), 0),
        assert_equal(r1->query_locked("door"), 0),
        assert_equal(testOb->query_locked("door"), 0),

        // lock doors
        r1->set_locked("east", 1),
        testOb->set_locked("west", 1),
        assert_equal(r1->query_locked("door"), 1),
        assert_equal(testOb->query_locked("door"), 1),
        assert_equal(r1->query_open("door"), 0),
        assert_equal(testOb->query_open("door"), 0),

        // can't open doors
        r1->set_open("east", 1),
        testOb->set_open("west", 1),
        assert_equal(r1->query_open("door"), 0),
        assert_equal(testOb->query_open("door"), 0),

        // unlock doors
        r1->set_locked("east", 0),
        testOb->set_locked("west", 0),
        assert_equal(r1->query_locked("door"), 0),
        assert_equal(testOb->query_locked("door"), 0),
        assert_equal(r1->query_open("door"), 0),
        assert_equal(testOb->query_open("door"), 0),
        // open previously locked doors
        r1->set_open("east", 1),
        testOb->set_open("west", 1),
        assert_equal(r1->query_open("door"), 1),
        assert_equal(testOb->query_open("door"), 1),
    }) :));

    expect("doors handle open, close, lock, and unlock", (: ({
        // close r1 door
        assert_equal(r1->handle_close(this_object(), "door"), 1),
        // already closed
        assert_equal(r1->handle_close(this_object(), "door"), -1),
        assert_equal(testOb->handle_close(this_object(), "door"), -1),
        // lock r1 door
        assert_equal(r1->handle_lock(this_object(), "door", "test key"), 1),
        // already locked
        assert_equal(r1->handle_lock(this_object(), "door", "test key"), -1),
        assert_equal(testOb->handle_lock(this_object(), "door", "test key"), -1),
        // can't open because locked
        assert_equal(testOb->handle_open(this_object(), "door"), -1),
        assert_equal(r1->handle_open(this_object(), "door"), -1),
        // unlock r1 door
        assert_equal(r1->handle_unlock(this_object(), "door", "test key"), 1),
        // already unlocked
        assert_equal(r1->handle_unlock(this_object(), "door", "test key"), -1),
        assert_equal(testOb->handle_unlock(this_object(), "door", "test key"), -1),
        // open r1 door
        assert_equal(r1->handle_open(this_object(), "door"), 1),
        // exit already open
        assert_equal(r1->handle_open(this_object(), "east"), -1),
        // already open
        assert_equal(r1->handle_open(this_object(), "door"), -1),
        assert_equal(testOb->handle_open(this_object(), "door"), -1),

        // close testOb door
        assert_equal(testOb->handle_close(this_object(), "door"), 1),
        // already closed
        assert_equal(testOb->handle_close(this_object(), "door"), -1),
        assert_equal(r1->handle_close(this_object(), "door"), -1),
        // lock testOb door
        assert_equal(testOb->handle_lock(this_object(), "door", "test key"), 1),
        // already locked
        assert_equal(testOb->handle_lock(this_object(), "door", "test key"), -1),
        assert_equal(r1->handle_lock(this_object(), "door", "test key"), -1),
        // unlock testOb door
        assert_equal(testOb->handle_unlock(this_object(), "door", "test key"), 1),
        // already unlocked
        assert_equal(testOb->handle_unlock(this_object(), "door", "test key"), -1),
        assert_equal(r1->handle_unlock(this_object(), "door", "test key"), -1),
        // open testOb door
        assert_equal(testOb->handle_open(this_object(), "door"), 1),
        // already open
        assert_equal(testOb->handle_open(this_object(), "door"), -1),
        assert_equal(r1->handle_open(this_object(), "door"), -1),

        // close and open testOb door by direction
        assert_equal(testOb->handle_close(this_object(), "west"), 1),
        assert_equal(testOb->handle_open(this_object(), "west"), 1),
    }) :));

    if (r1) destruct(r1);
}