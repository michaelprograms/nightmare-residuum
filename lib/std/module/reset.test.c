inherit M_TEST;

/**
 * @var {"/std/module/reset"} testOb
 */

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
        assert_equal(testOb->query_reset(), ([ "/std/item": 1 ])),
        testOb->reset(),
        assert_equal(testOb->query_resets(), 2),

        // set_reset will call reset for map argument
        testOb->set_reset(([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert_equal(testOb->query_resets(), 3),
        assert_equal(testOb->query_reset(), ([ "/std/item": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert_equal(setupFnCalled, 1),
        testOb->handle_reset(),
        assert_equal(testOb->query_resets(), 4),
        assert_equal(setupFnCalled, 2),

        // set_reset_data will not call reset for integer argument
        testOb->set_reset_data(([ "/std/item.c": 1 ])),
        assert_equal(testOb->query_resets(), 4),
        assert_equal(testOb->query_reset(), ([ "/std/item": 1 ])),
        // set_reset_data will not call reset for map argument
        testOb->set_reset_data(([ "/std/item.c": ([ "number": 1, "setup": $(setupFn) ]) ])),
        assert_equal(testOb->query_resets(), 4),
        assert_equal(testOb->query_reset(), ([ "/std/item": ([ "number": 1, "setup": $(setupFn) ]) ])),

        assert_catch((: testOb->set_reset_data(([ "": 0 ])) :), "*Bad reset data to reset->set_reset_data\n"),
    }) :));
}

/** @type {STD_NPC} npc */
nosave private object npc;
void test_objects () {
    object mockReset;
    /**
     * Called upon room's reset setup.
     * @param {STD_NPC} ob new npc
     */
    function setupFn = function (object ob) {
        npc = ob;
        npc->set_wander(1);
    };

    mockReset = new("/std/module/reset.mock.c");
    mockReset->start_shadow(testOb);

    expect("reset tracks wandering NPCs", (: ({
        // testOb will have initial reset from create
        testOb->reset(),
        assert_equal(testOb->query_resets(), 1),
        assert_equal(testOb->query_reset(), ([ ])),

        // set wanderer
        testOb->set_reset(([ "/std/npc.c": ([
            "number": 1,
            "setup": $(setupFn),
        ]) ])),
        assert_equal(testOb->query_resets(), 2),
        assert_equal(testOb->query_reset(), ([ "/std/npc": ([
            "number": 1,
            "setup": $(setupFn)
        ]) ])),

        // tracking object
        assert_equal(testOb->query_objects(), ([ "/std/npc": ({ npc }) ])),


        // still tracking wandering object
        assert_equal(testOb->query_objects(), ([ "/std/npc": ({ npc }) ])),

        // reset doesn't spawn another wandering NPC
        testOb->reset(),
        assert_equal(testOb->query_objects(), ([ "/std/npc": ({ npc }) ])),

        // remove wandering NPC
        npc->handle_remove(),
        // force a new NPC to spawn
        testOb->reset(),
        assert_equal(testOb->query_objects(), ([ "/std/npc": ({ npc }) ])),
    }) :));

    mockReset->stop_shadow();

    if (mockReset) destruct(mockReset);
    if (npc) destruct(npc);
}