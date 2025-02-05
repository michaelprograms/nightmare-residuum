// @this_object /std/living.c

int __Experience = 0, __TotalExperience = 0;
int __Victory = 0, __VictoryLevel = 0;
int __Defeated = 0;
mixed *__Defeat = ({ });

private void initialize_experience () {
    if (undefinedp(__Experience)) {
        __Experience = 0;
    }
    if (undefinedp(__TotalExperience)) {
        __TotalExperience = 0;
    }
}
private void initialize_victory () {
    if (undefinedp(__Victory)) {
        __Victory = 0;
    }
    if (undefinedp(__VictoryLevel)) {
        __VictoryLevel = 0;
    }
}
private void initialize_defeat () {
    if (undefinedp(__Defeated)) {
        __Defeated = 0;
    }
    if (!arrayp(__Defeat)) {
        __Defeat = ({ });
    }
}

int query_experience () {
    initialize_experience();
    return __Experience;
}
int query_total_experience () {
    initialize_experience();
    return __TotalExperience;
}
void add_experience (int exp) {
    if (undefinedp(exp) || !intp(exp)) {
        error("Bad argument 1 to biography->add_experience");
    }
    __Experience = __Experience + exp;
    // @TODO if (__Experience > ExpMax) __Experience = ExpMax
}
void spend_experience (int exp) {
    if (!intp(exp) || __Experience < exp || exp < 1) {
        error("Bad argument 1 to biography->spend_experience");
    }
    __Experience = __Experience - exp;
    __TotalExperience = __TotalExperience + exp;
}

int query_victory () {
    initialize_victory();
    return __Victory;
}
int query_victory_average () {
    if (!__Victory) {
        return 0;
    }
    return __VictoryLevel / __Victory;
}
mixed *query_defeat () {
    initialize_defeat();
    return __Defeat;
}
int query_defeated () {
    return __Defeated;
}
void set_defeated (int d) {
    __Defeated = d;
}

/**
 * When this living object defeats another living object, handle the victory
 * conditions.
 *
 * @param {STD_LIVING} source the object that was defeated by this object
 */
void handle_victory (object source) {
    int exp = D_EXPERIENCE->query_value(source);
    __Victory ++;
    __VictoryLevel += source->query_level();
    message("action", "You gain " + exp + " experience.", this_object());
    add_experience(exp);
}
/**
 * When this living object is defeated by another living object, handle the
 * defeat conditions.
 *
 * @param {STD_LIVING} source the object that defeated this object
 */
void handle_defeat (object source) {
    object env = environment(), corpse;

    initialize_defeat();
    if (__Defeated) {
        return;
    }

    this_object()->clear_protection();
    remove_call_out();

    this_object()->remove_hostile(source);
    if (source) {
        source->remove_hostile(this_object());
    }

    if (env) {
        corpse = new(STD_CORPSE);
        corpse->setup_body(this_object());
        corpse->handle_move(env);
        if (this_object()->query_cap_name()) {
            message("action", this_object()->query_cap_name() + " drops %^RED%^dead%^RESET%^.", env, this_object());
        }
    }

    __Defeated = 1;
    __Defeat += ({
        ({
            source ? source->query_cap_name() : 0,
            time()
        })
    });

    if (characterp(this_object())) {
        this_object()->handle_move("/domain/Nowhere/room/defeat.c");
    } else {
        this_object()->handle_remove();
    }
}