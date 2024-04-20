inherit M_TEST;

private nosave object testOb;
private nosave string testFile;
void before_all_tests () {
    testFile = D_TEST->create_coverage(replace_string(base_name(), ".test", ".c"));
}
void before_each_test () {
    testOb = clone_object(testFile);
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
}
void after_all_tests () {
    rm(testFile);
}
string *test_order () {
    return ({ "test_resets", "test_objects", });
}

private int resetFnCalled = 0;
private int setupFnCalled = 0;
void test_resets () {
    function setupFn = function (object ob) {
        setupFnCalled ++;
    };

    expect("resets handle setting, querying, and resetting", (: ({
        // have not set_reset yet
        assert_equal(testOb->query_resets(), 0),
        assert_equal(testOb->query_reset(), ([ ])),

        // set_reset will call reset for integer argument
        testOb->set_reset(([ "/std/item.c": 1 ])),
        assert_equal(testOb->query_resets(), 1),
        assert_equal(testOb->query_reset(), ([ "/std/item.c": 1 ])),
        testOb->reset(),
        assert_equal(testOb->query_resets(), 2),

        // set_reset will call reset for map argument
        testOb->set_reset(([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert_equal(testOb->query_resets(), 3),
        assert_equal(testOb->query_reset(), ([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert_equal(setupFnCalled, 1),
        testOb->reset(),
        assert_equal(testOb->query_resets(), 4),
        assert_equal(setupFnCalled, 2),

        // set_reset_data will not call reset for integer argument
        testOb->set_reset_data(([ "/std/item.c": 1 ])),
        assert_equal(testOb->query_resets(), 4),
        assert_equal(testOb->query_reset(), ([ "/std/item.c": 1 ])),
        // set_reset_data will not call reset for map argument
        testOb->set_reset_data(([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert_equal(testOb->query_resets(), 4),
        assert_equal(testOb->query_reset(), ([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
    }) :));
}

nosave private object r1, r2, npc;
void test_objects () {
    function setupFn = function (object ob) {
        npc = ob;
        npc->set_wander(1);
    };

    // setup test rooms
    r1 = new(STD_ROOM);
    r2 = new(STD_ROOM);
    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    expect("reset tracks wandering NPCs", (: ({
        // r1 will have initial reset from create
        assert_equal(r1->query_resets(), 1),
        assert_equal(r1->query_reset(), ([ ])),

        // set wanderer
        r1->set_reset(([ "/std/npc.c": ([
            "number": 1,
            "setup": $(setupFn),
        ]) ])),
        assert_equal(r1->query_resets(), 2),
        assert_equal(r1->query_reset(), ([ "/std/npc.c": ([
            "number": 1,
            "setup": $(setupFn)
        ]) ])),

        // tracking wandering object
        assert_equal(r1->query_objects(), ([ "/std/npc.c:0": npc ])),
        assert_equal(r2->query_objects(), ([ ])),

        // force NPC to wander to r2
        assert_equal(environment(npc), r1),
        npc->handle_wander(),
        assert_equal(environment(npc), r2),

        // still tracking wandering object
        assert_equal(r1->query_objects(), ([ "/std/npc.c:0": npc ])),
        assert_equal(r2->query_objects(), ([ ])),

        // reset doesn't spawn another wandering NPC
        r1->reset(),
        assert_equal(r1->query_objects(), ([ "/std/npc.c:0": npc ])),
        assert_equal(r2->query_objects(), ([ ])),

        // remove wandering NPC
        npc->handle_remove(),
        // force a new NPC to spawn
        r1->reset(),
        assert_equal(r1->query_objects(), ([ "/std/npc.c:0": npc ])),
        assert_equal(r2->query_objects(), ([ ])),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
    if (npc) destruct(npc);
}