inherit M_TEST;

/**
 * @var {"/secure/sefun/combat"} testOb
 */

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

void test_combat_messages () {
    object room;
    object npc1, npc2;
    object mockNpc1, mockNpc2;
    object weapon;

    room = new(STD_ROOM);
    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    npc1->set_name("npc one");
    npc2->set_name("npc two");
    npc1->set_stat("endurance", 10);
    npc2->set_stat("endurance", 10);
    npc1->handle_move(STD_ROOM);
    npc2->handle_move(STD_ROOM);
    mockNpc1 = new("/std/npc.mock.c");
    mockNpc2 = new("/std/npc.mock.c");
    mockNpc1->start_shadow(npc1);
    mockNpc2->start_shadow(npc2);
    weapon = new(STD_WEAPON);
    weapon->set_name("test weapon");
    weapon->set_type("blade");

    expect("combat hit messages should display", (: ({
        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", 1, 0, 0),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat hit", "You hit Npc two ineffectively in the limb with your weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat hit", "Npc one hits you ineffectively in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", $(weapon), 1, 0, 0),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat hit", "You hit Npc two ineffectively in the limb with your test weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat hit", "Npc one hits you ineffectively in the limb with their test weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", 123, 0, 0),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat hit", "You destroy Npc two utterly in the limb with your weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat hit", "Npc one destroys you utterly in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", 123, 0, 0),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat hit", "You destroy Npc two utterly in the limb with your weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat hit", "Npc one destroys you utterly in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", $(weapon), 123, 0, 0),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat hit", "You destroy Npc two utterly in the limb with your test weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat hit", "Npc one destroys you utterly in the limb with their test weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", 123, 1, 0),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat hit", "You critically destroy Npc two utterly in the limb with your weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat hit", "Npc one critically destroys you utterly in the limb with their weapon." })),

        testOb->combat_hit_message($(npc1), $(npc2), "limb", "weapon", 1, 0, 1),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "ability hit", "You hit Npc two ineffectively in the limb with your weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "ability hit", "Npc one hits you ineffectively in the limb with their weapon." })),
    }) :));

    mockNpc1->clear_received_messages();
    mockNpc2->clear_received_messages();

    expect("combat heal messages should display", (: ({
        // npc1 is full hp, no heal messages
        $(npc1)->set_hp($(npc1)->query_max_hp()),
        testOb->combat_heal_message($(npc2), $(npc1), "limb", 1),
        assert_equal($(mockNpc1)->query_received_messages(), ({ })),
        assert_equal($(mockNpc2)->query_received_messages(), ({ })),

        // npc2 at 1 hp, heal messages
        $(npc2)->set_hp(1),
        testOb->combat_heal_message($(npc1), $(npc2), "limb", 1),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat heal", "Npc two's wounds heal slightly." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat heal", "Your wounds heal slightly." })),
    }) :));

    mockNpc1->clear_received_messages();
    mockNpc2->clear_received_messages();

    expect("combat block messages should display", (: ({
        testOb->combat_block_message($(npc1), $(npc2)),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "Npc two blocks you." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "You block Npc one." })),
    }) :));

    mockNpc1->clear_received_messages();
    mockNpc2->clear_received_messages();

    expect("combat miss messages should display", (: ({
        testOb->combat_miss_message($(npc1), $(npc2), "WEAPON"),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "You miss Npc two with your WEAPON." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "Npc one misses you with their WEAPON." })),

        testOb->combat_miss_message($(npc1), $(npc2), $(weapon)),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "You miss Npc two with your test weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "Npc one misses you with their test weapon." })),
    }) :));

    mockNpc1->clear_received_messages();
    mockNpc2->clear_received_messages();

    expect("combat miss messages should display", (: ({
        testOb->combat_parry_message($(npc1), $(npc2), "WEAPON"),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "Npc two parries you with their WEAPON." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "You parry Npc one with your WEAPON." })),

        testOb->combat_parry_message($(npc1), $(npc2), $(weapon)),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "Npc two parries you with their test weapon." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "You parry Npc one with your test weapon." })),
    }) :));

    mockNpc1->clear_received_messages();
    mockNpc2->clear_received_messages();

    expect("combat evade messages should display", (: ({
        testOb->combat_evade_message($(npc1), $(npc2)),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "Npc two evades your attack." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "You evade Npc one's attack." })),

        testOb->combat_evade_message($(npc2), $(npc1)),
        assert_equal($(mockNpc1)->query_received_messages()[<1], ({ "combat miss", "You evade Npc two's attack." })),
        assert_equal($(mockNpc2)->query_received_messages()[<1], ({ "combat miss", "Npc one evades your attack." })),
    }) :));

    mockNpc1->clear_received_messages();
    mockNpc2->clear_received_messages();

    expect("combat useless messages should display", (: ({
        testOb->combat_useless_message($(npc1)),
        assert_equal($(mockNpc1)->query_received_messages()[<1][0], "combat miss"),
        assert_equal($(mockNpc2)->query_received_messages()[<1][0], "combat miss"),
    }) :));

    mockNpc1->stop_shadow();
    mockNpc2->stop_shadow();
    if (mockNpc1) destruct(mockNpc1);
    if (mockNpc2) destruct(mockNpc2);
    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
    if (room) destruct(room);
}

void test_combat_hit_damage () {
    object npc1, npc2;
    object weapon;
    npc1 = new(STD_NPC);
    npc2 = new(STD_NPC);
    npc1->set_level(10);
    npc2->set_level(1);
    npc1->set_stat("strength", 100);
    npc2->set_stat("endurance", 1);
    weapon = new(STD_WEAPON);
    weapon->set_name("test weapon");
    weapon->set_type("blade");

    expect("combat hit damage is calculated", (: ({
        assert_equal(testOb->combat_hit_damage($(npc1), $(npc2), "limb", "WEAPON", 0) >= 0, 1),
        assert_equal(testOb->combat_hit_damage($(npc1), $(npc2), "limb", $(weapon), 0) >= 0, 1),

        assert_equal(testOb->combat_hit_damage($(npc1), $(npc2), "limb", "WEAPON", 1) >= 0, 1),
        assert_equal(testOb->combat_hit_damage($(npc1), $(npc2), "limb", $(weapon), 1) >= 0, 1),
    }) :));

    if (npc1) destruct(npc1);
    if (npc2) destruct(npc2);
    if (weapon) destruct(weapon);
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

        assert_equal($(mockNpc1)->query_received_messages(), ({ ({ "attack", "You attack 0!" }) })),
        assert_equal($(mockNpc2)->query_received_messages(), ({ ({ "attack", "0 attacks you!" }) })),

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
    object living1 = new(STD_LIVING);
    object living2 = new(STD_LIVING);
    object weapon = new(STD_WEAPON);
    object armor = new(STD_ARMOR);

    living1->set_species("human");
    living2->set_species("human");
    weapon->set_type("blade");
    weapon->handle_move(living2);
    living2->handle_wield(weapon);
    armor->set_type("shield");
    armor->set_limbs(({ "left arm", "left hand" }));
    armor->handle_move(living2);
    living2->handle_wear(armor);

    expect("combat table is built", (: ({
        $(living1)->set_level(1),
        $(living2)->set_level(1),
        assert_equal(testOb->combat_table($(living1), $(living2), 0), ({
            ([ "id": "miss", "value": 5 ]),
            ([ "id": "block", "value": 5 ]),
            ([ "id": "parry", "value": 5 ]),
            ([ "id": "evade", "value": 5 ]),
            ([ "id": "critical hit", "value": 5 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 1), ({
            ([ "id": "miss", "value": 6 ]),
            ([ "id": "block", "value": 4 ]),
            ([ "id": "parry", "value": 4 ]),
            ([ "id": "evade", "value": 4 ]),
            ([ "id": "critical hit", "value": 4 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 2), ({
            ([ "id": "miss", "value": 7 ]),
            ([ "id": "block", "value": 3 ]),
            ([ "id": "parry", "value": 3 ]),
            ([ "id": "evade", "value": 3 ]),
            ([ "id": "critical hit", "value": 3 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        $(living1)->set_level(10),
        $(living2)->set_level(1),
        assert_equal(testOb->combat_table($(living1), $(living2), 0), ({
            ([ "id": "miss", "value": 3 ]),
            ([ "id": "block", "value": 3 ]),
            ([ "id": "parry", "value": 3 ]),
            ([ "id": "evade", "value": 3 ]),
            ([ "id": "critical hit", "value": 3 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 1), ({
            ([ "id": "miss", "value": 4 ]),
            ([ "id": "block", "value": 2 ]),
            ([ "id": "parry", "value": 2 ]),
            ([ "id": "evade", "value": 2 ]),
            ([ "id": "critical hit", "value": 2 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 2), ({
            ([ "id": "miss", "value": 5 ]),
            ([ "id": "block", "value": 1 ]),
            ([ "id": "parry", "value": 1 ]),
            ([ "id": "evade", "value": 1 ]),
            ([ "id": "critical hit", "value": 1 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        $(living1)->set_level(1),
        $(living2)->set_level(10),
        assert_equal(testOb->combat_table($(living1), $(living2), 0), ({
            ([ "id": "miss", "value": 6 ]),
            ([ "id": "block", "value": 6 ]),
            ([ "id": "parry", "value": 6 ]),
            ([ "id": "evade", "value": 6 ]),
            ([ "id": "critical hit", "value": 6 ]),
            ([ "id": "regular hit","value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 1), ({
            ([ "id": "miss", "value": 7 ]),
            ([ "id": "block", "value": 5 ]),
            ([ "id": "parry", "value": 5 ]),
            ([ "id": "evade", "value": 5 ]),
            ([ "id": "critical hit", "value": 5 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
        assert_equal(testOb->combat_table($(living1), $(living2), 2), ({
            ([ "id": "miss", "value": 8 ]),
            ([ "id": "block", "value": 4 ]),
            ([ "id": "parry", "value": 4 ]),
            ([ "id": "evade", "value": 4 ]),
            ([ "id": "critical hit", "value": 4 ]),
            ([ "id": "regular hit", "value": 100 ])
        })),
    }) :));

    if (living1) destruct(living1);
    if (living2) destruct(living2);
    if (weapon) destruct(weapon);
    if (armor) destruct(armor);
}