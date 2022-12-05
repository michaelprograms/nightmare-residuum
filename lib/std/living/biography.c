#include "living.h"

private int __Experience = 0, __TotalExperience = 0;
private int __Victory = 0, __VictoryLevel;
private mixed *__Defeat = ({ });


int query_experience () {
    if (undefinedp(__Experience)) __Experience = 0;
    return __Experience;
}
int query_total_experience () {
    if (undefinedp(__TotalExperience)) __TotalExperience = 0;
    return __TotalExperience;
}
void add_experience (int exp) {
    if (!intp(exp)) error("Bad argument 1 to body->add_experience");
    __Experience = __Experience + exp;
    // @TODO if (__Experience > ExpMax) __Experience = ExpMax
}
void spend_experience (int exp) {
    if (!intp(exp) || __Experience < exp || exp < 1) error("Bad argument 1 to body->spend_experience");
    __Experience = __Experience - exp;
    __TotalExperience = __TotalExperience + exp;
}

int query_victory () {
    if (undefinedp(__Victory)) __Victory = 0;
    return __Victory;
}
int query_victory_average () {
    if (!__Victory) return 0;
    return __VictoryLevel / __Victory;
}
mixed *query_defeat () {
    if (!arrayp(__Defeat)) __Defeat = ({ });
    return __Defeat;
}
void handle_victory (object source) {
    int exp = D_EXPERIENCE->query_value(source);
    __Victory ++;
    __VictoryLevel += source->query_level();
    message("action", "You gain " + exp + " experience.", this_object());
    add_experience(exp);
}
void handle_defeat (object source) {
    object env = environment(), corpse;

    if (!arrayp(__Defeat)) {
        __Defeat = ({ });
    }

    __Defeat += ({
        ({
            source ? source->query_cap_name() : 0,
            time()
        })
    });

    if (env) {
        corpse = new(ITEM_CORPSE);
        corpse->setup_body(this_object());
        corpse->handle_move(env);
        if (this_object()->query_cap_name()) {
            message("action", this_object()->query_cap_name() + " drops %^RED%^dead%^RESET%^.", env, this_object());
        }
    }

    if (this_object()->is_character()) this_object()->handle_move("/domain/Nowhere/room/defeat.c");
    else this_object()->handle_remove();
}