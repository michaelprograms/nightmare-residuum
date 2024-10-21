inherit M_TEST;

void test_combat_tier_from_percent () {
    expect("combat message tiers behave", (: ({
        assert_equal(testOb->query_combat_tier_from_percent(-1), 0),
        assert_equal(testOb->query_combat_tier_from_percent(0), 0),
        assert_equal(testOb->query_combat_tier_from_percent(1), 0),

        assert_equal(testOb->query_combat_tier_from_percent(2), 1),

        assert_equal(testOb->query_combat_tier_from_percent(3), 2),

        assert_equal(testOb->query_combat_tier_from_percent(4), 3),
        assert_equal(testOb->query_combat_tier_from_percent(5), 3),

        assert_equal(testOb->query_combat_tier_from_percent(6), 4),
        assert_equal(testOb->query_combat_tier_from_percent(7), 4),

        assert_equal(testOb->query_combat_tier_from_percent(8), 5),
        assert_equal(testOb->query_combat_tier_from_percent(9), 5),

        assert_equal(testOb->query_combat_tier_from_percent(10), 6),
        assert_equal(testOb->query_combat_tier_from_percent(12), 6),

        assert_equal(testOb->query_combat_tier_from_percent(13), 7),
        assert_equal(testOb->query_combat_tier_from_percent(15), 7),

        assert_equal(testOb->query_combat_tier_from_percent(16), 8),
        assert_equal(testOb->query_combat_tier_from_percent(18), 8),

        assert_equal(testOb->query_combat_tier_from_percent(19), 9),
        assert_equal(testOb->query_combat_tier_from_percent(22), 9),

        assert_equal(testOb->query_combat_tier_from_percent(23), 10),
        assert_equal(testOb->query_combat_tier_from_percent(26), 10),

        assert_equal(testOb->query_combat_tier_from_percent(27), 11),
        assert_equal(testOb->query_combat_tier_from_percent(100), 11),
        assert_equal(testOb->query_combat_tier_from_percent(12345), 11),
    }) :));
}

void test_combat_hit_message () {
    object npc1, npc2;
    object mockNpc1, mockNpc2;

    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    npc1->set_level(1);
    npc2->set_level(1);
    npc1->set_name("npc one");
    npc2->set_name("npc two");
    mockNpc1 = new("/std/npc.mock.c");
    mockNpc2 = new("/std/npc.mock.c");

    expect("messages should display", (: ({
        assert_equal($(mockNpc1)->start_shadow($(npc1)), 1),
        assert_equal($(mockNpc2)->start_shadow($(npc2)), 1),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", "blunt", 1, 0, 0),
        assert_equal($(npc1)->query_received_messages()[<1], ({ "combat hit", "You hit Npc two ineffectively in the limb with your weapon." })),
        assert_equal($(npc2)->query_received_messages()[<1], ({ "combat hit", "Npc one hits you ineffectively in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", "blunt", 123, 0, 0),
        assert_equal($(npc1)->query_received_messages()[<1], ({ "combat hit", "You destroy Npc two utterly in the limb with your weapon." })),
        assert_equal($(npc2)->query_received_messages()[<1], ({ "combat hit", "Npc one destroys you utterly in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", "blade", 123, 0, 0),
        assert_equal($(npc1)->query_received_messages()[<1], ({ "combat hit", "You destroy Npc two utterly in the limb with your weapon." })),
        assert_equal($(npc2)->query_received_messages()[<1], ({ "combat hit", "Npc one destroys you utterly in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", "blade", 123, 1, 0),
        assert_equal($(npc1)->query_received_messages()[<1], ({ "combat hit", "You critically destroy Npc two utterly in the limb with your weapon." })),
        assert_equal($(npc2)->query_received_messages()[<1], ({ "combat hit", "Npc one critically destroys you utterly in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", "blunt", 1, 0, 1),
        assert_equal($(npc1)->query_received_messages()[<1], ({ "ability hit", "You hit Npc two ineffectively in the limb with your weapon." })),
        assert_equal($(npc2)->query_received_messages()[<1], ({ "ability hit", "Npc one hits you ineffectively in the limb with their weapon." })),

        assert_equal($(mockNpc1)->stop_shadow(), 1),
        assert_equal($(mockNpc2)->stop_shadow(), 1),
    }) :));

    if (mockNpc1) destruct(mockNpc1);
    if (mockNpc2) destruct(mockNpc2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
}

void test_initiate_combat () {
    object npc1, npc2;
    object mockNpc1, mockNpc2;

    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    mockNpc1 = new("/std/npc.mock.c");
    mockNpc2 = new("/std/npc.mock.c");

    expect("hostilities should ensue", (: ({
        assert_equal($(mockNpc1)->start_shadow($(npc1)), 1),
        assert_equal($(mockNpc2)->start_shadow($(npc2)), 1),

        assert_equal($(npc1)->query_hostiles(), ({ })),
        assert_equal($(npc2)->query_hostiles(), ({ })),

        testOb->initiate_combat($(npc1), $(npc2)),
        assert_equal($(npc1)->query_hostiles(), ({ $(npc2) })),
        assert_equal($(npc2)->query_hostiles(), ({ $(npc1) })),

        assert_equal($(npc1)->query_received_messages(), ({ ({ "attack", "You attack 0!" }) })),
        assert_equal($(npc2)->query_received_messages(), ({ ({ "attack", "0 attacks you!" }) })),

        assert_equal($(mockNpc1)->stop_shadow(), 1),
        assert_equal($(mockNpc2)->stop_shadow(), 1),
    }) :));

    if (mockNpc1) destruct(mockNpc1);
    if (mockNpc2) destruct(mockNpc2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
}

void test_present_hostile () {
    object room, living1, living2;

    room = new(STD_ROOM);
    living1 = new(STD_LIVING);
    living2 = new(STD_LIVING);
    living1->handle_move(room);
    living2->handle_move(room);

    expect("present hostiles are returned", (: ({
        assert_equal(testOb->present_hostiles($(living1)), ({ })),
        assert_equal(testOb->present_hostile($(living1)), 0),
        assert_equal(testOb->present_hostiles($(living2)), ({ })),
        assert_equal(testOb->present_hostile($(living2)), 0),

        $(living1)->add_hostile($(living2)),
        $(living2)->add_hostile($(living1)),

        assert_equal(testOb->present_hostiles($(living2)), ({ $(living1) })),
        assert_equal(testOb->present_hostile($(living2)), $(living1)),
        assert_equal(testOb->present_hostiles($(living1)), ({ $(living2) })),
        assert_equal(testOb->present_hostile($(living1)), $(living2)),
    }) :));

    if (room) destruct(room);
    if (living1) destruct(living1);
    if (living2) destruct(living2);
}

void test_combat_table () {
    object living1 = new("/std/living.c");
    object living2 = new("/std/living.c");

    expect("combat table is built", (: ({
        $(living1)->set_level(1),
        $(living2)->set_level(1),
        assert_equal(testOb->combat_table($(living1), $(living2), 0), ({
            ([ "id": "miss", "value": 5 ]),
            ([ "id": "evade", "value": 5 ]),
            ([ "id": "critical hit", "value": 5 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 1), ({
            ([ "id": "miss", "value": 6 ]),
            ([ "id": "evade", "value": 4 ]),
            ([ "id": "critical hit", "value": 4 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 2), ({
            ([ "id": "miss", "value": 7 ]),
            ([ "id": "evade", "value": 3 ]),
            ([ "id": "critical hit", "value": 3 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        $(living1)->set_level(10),
        $(living2)->set_level(1),
        assert_equal(testOb->combat_table($(living1), $(living2), 0), ({
            ([ "id": "miss", "value": 3 ]),
            ([ "id": "evade", "value": 3 ]),
            ([ "id": "critical hit", "value": 3 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 1), ({
            ([ "id": "miss", "value": 4 ]),
            ([ "id": "evade", "value": 2 ]),
            ([ "id": "critical hit", "value": 2 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 2), ({
            ([ "id": "miss", "value": 5 ]),
            ([ "id": "evade", "value": 1 ]),
            ([ "id": "critical hit", "value": 1 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        $(living1)->set_level(1),
        $(living2)->set_level(10),
        assert_equal(testOb->combat_table($(living1), $(living2), 0), ({
            ([ "id": "miss", "value": 6 ]),
            ([ "id": "evade", "value": 6 ]),
            ([ "id": "critical hit", "value": 6 ]),
            ([ "id": "regular hit","value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 1), ({
            ([ "id": "miss", "value": 7 ]),
            ([ "id": "evade", "value": 5 ]),
            ([ "id": "critical hit", "value": 5 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 2), ({
            ([ "id": "miss", "value": 8 ]),
            ([ "id": "evade", "value": 4 ]),
            ([ "id": "critical hit", "value": 4 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
    }) :));

    if (living1) destruct(living1);
    if (living2) destruct(living2);
}