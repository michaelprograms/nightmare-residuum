inherit M_TEST;

mapping *query_achievements () {
    return ({
        ([
            "name": "test",
            "description": "Test.",
            "flags": 1,
        ])
    });
}

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