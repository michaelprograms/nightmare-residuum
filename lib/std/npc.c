inherit STD_LIVING;

nosave private string *__AbilityList = ({ });
nosave private int __AbilityChance = 0;
nosave private int __Aggressive;
nosave private int __Wander = 0, __Wanders = 0, __NextWander = 0, __Wandering = 0;
nosave private mapping __SayResponse = ([ ]);

int is_npc () {
    return 1;
}

void set_say_response (string match, string response) {
    __SayResponse[match] = response;
}
mapping query_say_response () {
    return copy(__SayResponse);
}
string *query_say_response_matches () {
    return keys(__SayResponse);
}

void handle_say_response (string message) {
    foreach (string match, string response in __SayResponse) {
        if (regexp(message, match)) {
            this_object()->handle_command("say " + response);
            return;
        }
    }
}

void receive_message (string type, string message) {
    if (type == "say") {
        if (regexp(message, "^You (ask|exclaim|synthesize|say):")) {
            return;
        }
        handle_say_response(message);
    }
}

void set_level (int l) {
    int n = l + 1;

    set_stat("strength", l + n * 4);
    set_stat("perception", l + n * 5);
    set_stat("endurance", l + n * 5);
    set_stat("charisma", l + n * 3);
    set_stat("intelligence", l + n * 4);
    set_stat("agility", l + n * 5);
    set_stat("luck", to_int(l + n * 2.5) + random(n + 1));

    ::set_level(l);
}

string *query_ability_list () {
    return __AbilityList;
}
void set_ability_list (string *abilities) {
    __AbilityList = abilities;
}
int query_ability_chance () {
    return __AbilityChance;
}
void set_ability_chance (int chance) {
    __AbilityChance = max(({ 0, min(({ chance, 100 })) }));
}
void handle_ability_attack () {
    if (!sizeof(__AbilityList)) {
        return;
    }
    if (__AbilityChance > 1 + random(100)) {
        this_object()->handle_command(element_of(__AbilityList));
    }
}

int query_aggressive () {
    return __Aggressive;
}
void set_aggressive (int n) {
    __Aggressive = n;
}

nosave private mapping __Inventory = ([ ]);
void set_inventory (mapping inventory) {
    if (!mapp(inventory)) {
        error("Bad argument 1 to npc->set_inventory");
    }

    __Inventory = inventory;
}
mapping query_inventory () {
    return __Inventory;
}

void handle_received (object env) {
    ::handle_received(env);

    if (sizeof(__Inventory)) {
        foreach (string item,string command in __Inventory) {
            new(item)->handle_move(this_object());
            handle_command(command);
        }
        __Inventory = ([ ]);
    }
}

/**
 * Handle receiving a living object in the same environment as this NPC.
 *
 * @param {STD_LIVING} living the living object entering the environment
 */
void handle_receive_living_in_env (object living) {
    if (characterp(living)) {
        // auto attack characters
        if (
            __Aggressive > 0 &&
            !living->query_defeated() &&
            living->query_stat("charisma") < __Aggressive &&
            !query_hostile(living)
        ) {
            initiate_combat(this_object(), living);
        }

        // reset wander count
        if (__Wander && !__Wandering) {
            __Wanders = 0;
        }
    }
}

/* ----- wander ----- */

int query_next_wander () {
    return __NextWander;
}
void set_next_wander (int n) {
    __NextWander = n;
}
int query_wander () {
    return __Wander;
}
/**
 * Enables wandering on this NPC.
 * @param n the interval of rounds between wander attempts
 */
void set_wander (int n) {
    __Wander = n;
    __NextWander = 0;
}
int query_wanders () {
    return __Wanders;
}

void handle_wander () {
    /** @type {STD_ROOM} env */
    object env;
    string *exits;

    if (!this_object() || !(env = environment())) {
        return;
    }

    exits = env->query_exit_directions();
    if (sizeof(exits) < 1) {
        return;
    }

    __Wandering = 1;
    handle_command("go " + element_of(exits));
    call_out_walltime(function () {
        __Wandering = 0;
    }, 0);
}

void attempt_wander () {
    if (__Wander) {
        if (__NextWander >= __Wander && __Wanders < 20) {
            handle_wander();
            __NextWander = 0;
            __Wanders ++;
        } else if (__NextWander < 20) {
            __NextWander ++;
        }
    }
}

/* ----- applies ----- */

void create () {
    ::create();
    if (!clonep()) {
        return;
    }
    set_heart_beat(1);
}

void heart_beat () {
    if (!clonep()) {
        return;
    }
    if (random(2) && member_array(query_posture(), ({ "sitting", "laying" })) > -1 && present_hostile(this_object())) {
        handle_command("stand");
    }
    ::heart_beat();

    attempt_wander();
}