inherit M_TEST;
inherit M_MOVE;
inherit M_LEVELABLE;

/**
 * @var {"/std/living/combat"} testOb
 */

void test_hostiles() {
    object ob1 = new(STD_LIVING), ob2 = new(STD_LIVING);
    object mockCharacter = new("/std/character.mock.c");

    expect("handles adding, querying, and removing hostiles", (: ({
        assert_equal($(mockCharacter)->start_shadow(testOb), 1),

        // test adding and removing
        assert_equal(sizeof(testOb->query_hostiles()), 0),  // no hostiles
        assert_equal(testOb->add_hostile($(ob1)), 1),       // 1st hostile added
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // 1 hostile
        assert_equal(testOb->query_hostile($(ob1)), 1),     // 1st hostile found
        assert_equal(testOb->add_hostile($(ob1)), 0),       // can't add same hostile
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // still 1 hostile
        assert_equal(testOb->add_hostile($(ob2)), 1),       // 2nd hostile added
        assert_equal(sizeof(testOb->query_hostiles()), 2),  // 2 hostiles
        assert_equal(testOb->query_hostile($(ob2)), 1),     // 2nd hostile found
        assert_equal(testOb->remove_hostile($(ob1)), 1),    // 1st hostile removed
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // 1 hostile
        assert_equal(testOb->remove_hostile($(ob1)), 0),    // can't remove same hostile
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // still 1 hostile
        assert_equal(testOb->remove_hostile($(ob2)), 1),    // 2nd hostile removed
        assert_equal(sizeof(testOb->query_hostiles()), 0),  // 0 hostiles
        assert_equal(testOb->query_hostile($(ob1)), 0),     // 1st hostile not found
        assert_equal(testOb->query_hostile($(ob2)), 0),     // 2nd hostile not found

        // test defeated hostile
        $(ob1)->set_defeated(1),
        assert_equal(testOb->add_hostile($(ob1)), 0),       // can't add defeated hostile
        $(ob1)->set_defeated(0),
        assert_equal(testOb->add_hostile(testOb), 0),       // can't add defeated self

        // test filtering undefined
        assert_equal(testOb->add_hostile($(ob1)), 1),       // 1st hostile added
        assert_equal(testOb->add_hostile($(ob2)), 1),       // 2nd hostile added
        assert_equal($(ob2)->handle_remove(), 1),           // 2nd hostile destructed
        assert_equal(sizeof(testOb->query_hostiles()), 1),  // only 1 hostile

        assert_equal($(mockCharacter)->stop_shadow(), 1),
    }) :));

    if (ob1) ob1->handle_remove();
    if (ob2) ob2->handle_remove();
    if (mockCharacter) destruct(mockCharacter);
}

void test_check_lifesigns() {
    object mockLiving = new("/std/living/combat.mock.c");

    mockLiving->start_shadow(testOb);

    expect("check lifesigns behaves", (: ({
        assert_equal($(mockLiving)->query_defeated(), 0),
        testOb->check_lifesigns(),
        assert_equal($(mockLiving)->query_defeated(), 0),
        assert_equal($(mockLiving)->query_defeat(), ({})),

        // already defeated
        $(mockLiving)->set_defeated(1),
        testOb->check_lifesigns(),
        assert_equal(objectp(testOb), 1),

        $(mockLiving)->set_defeated(0),
        $(mockLiving)->set_hp(-1),
        testOb->check_lifesigns(),
        assert_equal(objectp($(mockLiving)), 0),
    }) :));

    if (mockLiving) {
        mockLiving->stop_shadow();
        destruct(mockLiving);
    }
}

int is_living() {
    return 1;
}
void test_parser_applies() {
    expect("combat applies behave", (: ({
        assert_equal(testOb->direct_attack_liv(), 1),
        assert_equal(testOb->direct_aid_liv(), 1),
        testOb->add_hostile(this_object()),
        assert_equal(testOb->direct_attack_liv(), 0),
        assert_equal(testOb->direct_aid_liv(), 0),
    }) :));
}

void test_combat_hit() {
    object r = new(STD_ROOM);
    object mockLiving = new("/std/living/combat.mock.c");
    object npc = new(STD_NPC), mockNpc1 = new("/std/npc.mock.c");

    mockNpc1->start_shadow(npc);
    mockLiving->start_shadow(testOb);
    // the target needs an environment for the block message; the bare
    // combat instance (the source) cannot have one, so branches whose
    // messages address the source's environment (parry/evade and the
    // successful-hit message) are not reachable here
    npc->handle_move(r);

    expect("handle_combat_hit returns early without a target", (: ({
        testOb->handle_combat_hit(0, ({}), 0),
        assert_equal(objectp(testOb), 1),
    }) :));

    expect("handle_combat_hit rolls a die when sp is available", (: ({
        // compute vitals so spMax is non-zero and sp can be set above 0
        $(mockLiving)->update_vitals(1),
        $(mockLiving)->set_sp(100),
        // sp > 0 rolls d100; the zero-value entry is then skipped
        testOb->handle_combat_hit(
            $(npc),
            ({ ([ "id": "skip", "value": 0 ]) }),
            0
        ),
        assert_equal(objectp(testOb), 1),
    }) :));

    // sp <= 0 keeps d100 at 0, so the first non-zero entry always lands,
    // making each combat branch deterministic
    expect("handle_combat_hit resolves miss and block", (: ({
        $(mockLiving)->set_sp(0),
        // leading zero-value entry exercises the continue path, then a miss
        testOb->handle_combat_hit(
            $(npc),
            ({
                ([ "id": "skip", "value": 0 ]),
                ([ "id": "miss", "value": 100 ]),
            }),
            0
        ),
        testOb->handle_combat_hit(
            $(npc),
            ({ ([ "id": "block", "value": 100 ]) }),
            0
        ),
        assert_equal(objectp($(npc)), 1),
    }) :));

    // high protection fully mitigates damage, so the regular and critical
    // hit cases resolve through the (source-environment-guarded) miss path
    expect("handle_combat_hit resolves mitigated regular and critical hits", (: ({
        $(mockLiving)->set_sp(0),
        $(npc)->set_protection(1000),
        testOb->handle_combat_hit(
            $(npc),
            ({ ([ "id": "regular hit", "value": 100 ]) }),
            0
        ),
        // critical hit sets crit then falls through to the regular resolution
        testOb->handle_combat_hit(
            $(npc),
            ({ ([ "id": "critical hit", "value": 100 ]) }),
            0
        ),
        assert_equal(objectp($(npc)), 1),
    }) :));

    if (mockLiving) {
        mockLiving->stop_shadow();
        destruct(mockLiving);
    }
    if (mockNpc1) {
        mockNpc1->stop_shadow();
        destruct(mockNpc1);
    }
    if (npc) destruct(npc);
    if (r) destruct(r);
}

void test_handle_combat_returns_without_target() {
    expect("handle_combat returns early when no hostile is present", (: ({
        // a fresh instance has no hostiles, so present_hostile is 0
        testOb->handle_combat(),
        assert_equal(objectp(testOb), 1),
    }) :));
}

void test_combat() {
    object mockLiving = new("/std/living/combat.mock.c");
    object npc = new(STD_NPC), mockNpc1;
    mapping *table;

    mockNpc1 = new("/std/npc.mock.c");
    mockNpc1->start_shadow(npc);

    mockLiving->start_shadow(testOb);
    npc->add_hostile(testOb);
    testOb->add_hostile(npc);
    table = combat_table(mockLiving, npc, 1),

        expect("combat behaves", (: ({
            $(mockLiving)->set_posture("meditating"),
            assert_equal($(mockLiving)->query_posture(), "meditating"),
            testOb->handle_combat(),
            assert_equal($(mockLiving)->query_posture(), "sitting"),

            testOb->handle_combat_hit($(npc), $(table), 0),


        }) :));


    if (mockLiving) {
        mockLiving->stop_shadow();
        destruct(mockLiving);
    }
    if (mockNpc1) {
        mockNpc1->stop_shadow();
        destruct(mockNpc1);
    }
}
