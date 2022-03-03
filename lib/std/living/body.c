#include "living.h"

private string __Gender = "neither";
private int __Level = 0;
private int __Experience = 0;
private int __TotalExperience = 0;
private int __Victory = 0;
private int __Defeat = 0;

string query_gender () {
    return __Gender;
}
void set_gender (string str) {
    switch(str) {
    case "male": case "female": case "neither":
        __Gender = str;
        break;
    default:
        __Gender = "none";
    }
}

int query_level () {
    return __Level;
}
void set_level (int l) {
    if (l < 0) {
        l = abs(l);
    }
    __Level = l;
}

int query_experience () {
    if (undefinedp(__Experience)) __Experience = 0;
    return __Experience;
}
int query_total_experience () {
    if (undefinedp(__TotalExperience)) __TotalExperience = 0;
    return __TotalExperience;
}
void add_experience (int exp) {
    __Experience = __Experience + exp;
    // @TODO if (__Experience > ExpMax) __Experience = ExpMax
}
void spend_experience (int exp) {
    if (!intp(exp) || __Experience < exp || exp < 1) error("Bad argument 1 to body->spend_experience");
    __Experience = __Experience - exp;
    __TotalExperience = __TotalExperience + exp;
}

void add_victory () {
    __Victory = __Victory + 1;
}
void add_defeat () {
    __Defeat = __Defeat + 1;
}
int query_victory () {
    if (undefinedp(__Victory)) __Victory = 0;
    return __Victory;
}
int query_defeat () {
    if (undefinedp(__Defeat)) __Defeat = 0;
    return __Defeat;
}
void handle_victory (object source) {
    int exp = D_EXPERIENCE->query_value(source);
    message("system", "You gain " + exp + " experience.\n", this_object());
    add_experience(exp);
    add_victory();
}
void handle_defeat (int keep) {
    add_defeat();

    new("/std/item/corpse.c")->handle_move(environment(this_object()));

    if (keep) this_object()->handle_move("/domain/Nowhere/room/defeat.c");
    else this_object()->handle_remove();
}