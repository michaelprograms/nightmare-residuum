#include "living.h"

private mapping __Stats = ([
    "strength": 0,
    "perception": 0,
    "endurance": 0,
    "charisma": 0,
    "intelligence": 0,
    "agility": 0,
    "luck": 0,
]);

int query_stat (string stat) {
    return __Stats[stat];
}
void set_stat (string stat, int n) {
    // @TODO security
    __Stats[stat] = n;
}