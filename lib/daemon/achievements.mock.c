inherit "/std/shadow.c";

mapping *__MockAchievements = ({});
void set_mock_achievements(mapping *a) {
    __MockAchievements = a;
}
mapping *query_achievements_from_room(mixed _dest) {
    return __MockAchievements;
}
