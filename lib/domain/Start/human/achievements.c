mapping *achievements = ({
    ([
        "name": "Art Appreciation",
        "description": "Explore the museum's hallways and appreciate the works of art.",
        "flags": "p1,p2,p3,p4,s1,s2,s3,s4"
    ]),
});
mapping *query_achievements () {
    return achievements;
}

string query_achievement_flags (string str) {
    mapping *match = filter(achievements, (: $1["name"] == $(str) :));
    return sizeof(match) ? match[0]["flags"] : 0;
}