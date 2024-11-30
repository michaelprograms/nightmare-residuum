inherit "/std/shadow.c";

mapping *__MockAchievements = ({ });
void set_mock_achievements (mapping *a) {
    __MockAchievements = a;
}
mapping query_achievements_from_room (mixed dest) {
    return __MockAchievements[0];
}