inherit M_TEST;
inherit M_MOVE;
inherit M_LEVELABLE;

void test_hostiles () {
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

int is_living () {
    return 1;
}
void test_parser_applies () {
    expect("combat applies behave", (: ({
        assert_equal(testOb->direct_attack_liv(), 1),
        assert_equal(testOb->direct_aid_liv(), 1),
        testOb->add_hostile(this_object()),
        assert_equal(testOb->direct_attack_liv(), 0),
        assert_equal(testOb->direct_aid_liv(), 0),
    }) :));
}

void test_combat_table () {
    object mockBody = new("/std/living/body.mock.c");

    expect("combat table is built", (: ({
        assert_equal($(mockBody)->start_shadow(testOb), 1),

        assert_equal(testOb->query_combat_table(this_object()), ({
            ([ "id": "miss", "value": 5.000000 ]),
            ([ "id": "resist", "value": 0.000000 ]),
            ([ "id": "block", "value": 0.000000 ]),
            ([ "id": "parry", "value": 0.000000 ]),
            ([ "id": "evade", "value": 5.000000 ]),
            ([ "id": "critical hit", "value": 5.000000 ]),
            ([ "id": "regular hit", "value": 100.000000 ])
        })),
        testOb->set_level(1),
        set_level(1),
        assert_equal(testOb->query_combat_table(this_object()), ({
            ([ "id": "miss", "value": 5.000000 ]),
            ([ "id": "resist", "value": 0.000000 ]),
            ([ "id": "block", "value": 0.000000 ]),
            ([ "id": "parry", "value": 0.000000 ]),
            ([ "id": "evade", "value": 5.000000 ]),
            ([ "id": "critical hit", "value": 5.000000 ]),
            ([ "id": "regular hit", "value": 100.000000 ])
        })),
        testOb->set_level(10),
        assert_equal(testOb->query_combat_table(this_object()), ({
            ([ "id": "miss", "value": 3.200000 ]),
            ([ "id": "resist", "value": 0.000000 ]),
            ([ "id": "block", "value": 0.000000 ]),
            ([ "id": "parry", "value": 0.000000 ]),
            ([ "id": "evade", "value": 3.200000 ]),
            ([ "id": "critical hit", "value": 3.200000 ]),
            ([ "id": "regular hit", "value": 100.000000 ])
        })),
        testOb->set_level(1),
        set_level(10),
        assert_equal(testOb->query_combat_table(this_object()), ({
            ([ "id": "miss", "value": 6.800000 ]),
            ([ "id": "resist", "value": 0.000000 ]),
            ([ "id": "block", "value": 0.000000 ]),
            ([ "id": "parry", "value": 0.000000 ]),
            ([ "id": "evade", "value": 6.800000 ]),
            ([ "id": "critical hit", "value": 6.800000 ]),
            ([ "id": "regular hit","value": 100.000000 ])
        })),

        assert_equal($(mockBody)->stop_shadow(testOb), 1),
    }) :));

    if (mockBody) destruct(mockBody);
}