inherit M_TEST;

/**
 * @var {"/daemon/achievements"} testOb
 */

#define ACHIEVEMENTS_MOCK "/daemon/achievements.c" & "/daemon/achievements.mock.c"

void test_query_achievements_from_room () {
    object room1 = load_object("/domain/Start/human/room/square.c");
    object room2 = load_object("/domain/Start/human/void.c");

    expect("query_achievements_from_room finds achievements", (: ({
        assert_equal(testOb->query_achievements_from_room("/domain/Start/human/room/square.c"), ({ ([ "description": "Explore the museum's hallways and appreciate the works of art.", "flags": "p1,p2,p3,p4,s1,s2,s3,s4", "name": "Art Appreciation" ]) })),
        assert_equal(testOb->query_achievements_from_room($(room1)), ({ ([ "description": "Explore the museum's hallways and appreciate the works of art.", "flags": "p1,p2,p3,p4,s1,s2,s3,s4", "name": "Art Appreciation" ]) })),

        assert_equal(testOb->query_achievements_from_room("/domain/Nowhere/room/void.c"), 0),
        assert_equal(testOb->query_achievements_from_room($(room2)), 0),
    }) :));
}

string __Name, __Flag, __Flags;
void set_achievement_flag (string name, string flag, string flags) {
    __Name = name;
    __Flag = flag;
    __Flags = flags;
}
void test_flag () {
    object mockAchievements = new("/daemon/achievements.mock.c");

    expect("flag sets", (: ({
        assert_equal($(mockAchievements)->start_shadow(testOb), 1),

        /** @type {ACHIEVEMENTS_MOCK} */ (testOb)->set_mock_achievements(({
            ([
                "name": "test",
                "description": "Test achievement.",
                "flags": "a,b,c,1,2,3"
            ]),
        })),

        assert_equal(__Name, UNDEFINED),
        assert_equal(__Flag, UNDEFINED),
        assert_equal(__Flags, UNDEFINED),

        testOb->flag(this_object(), "test", "a"),
        assert_equal(__Name, "test"),
        assert_equal(__Flag, "a"),
        assert_equal(__Flags, "a,b,c,1,2,3"),

        testOb->flag(this_object(), "test", "b"),
        assert_equal(__Name, "test"),
        assert_equal(__Flag, "b"),
        assert_equal(__Flags, "a,b,c,1,2,3"),

        testOb->flag(this_object(), "test", "z"),
        assert_equal(__Name, "test"),
        assert_equal(__Flag, "z"),
        assert_equal(__Flags, "a,b,c,1,2,3"),

        assert_equal($(mockAchievements)->stop_shadow(), 1),
    }) :));

    if (mockAchievements) destruct(mockAchievements);
}