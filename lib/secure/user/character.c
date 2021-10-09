#include "user.h"

nosave private string __BodyType;
nosave private object __Character;
private int __Immortal;

// -----------------------------------------------------------------------------

private void set_character_name (string name) {
    if (!__Character) {
        __Character = clone_object(STD_CHARACTER);
        __Character->set_user(this_object());
    }
    if (name) {
        __Character->set_name(capitalize(name));
    } else {
        destruct(__Character);
        __Character = clone_object(STD_CHARACTER);
    }
}
private void set_character_type (string type) {
    __BodyType = type;
}
string query_character_type () {
    return __BodyType;
}
object query_character () {
    return __Character;
}

mapping query_character_summary () { // @TODO deprecrate this and just pass __Character?
    if (!__Character) return ([]);
    return ([
        "type": __Character->query_type(), // @TODO?
        "level": __Character->query_level(),
        "last_action": __Character->query_last_action(),
        "last_location": __Character->query_last_location(),
    ]);
}

int query_immortal () { return __Immortal; }
void set_immortal (int i) {
    // @TODO security
    __Immortal = i;
}

nomask private void setup_character (int newbie) {

}

// -----------------------------------------------------------------------------

nomask private void character_enter (int newbie) {
    remove_call_out();
    // @TODO FTUE
    __Character->setup();

    if (newbie) {
        if (__Character->query_type() == "human") {
            __Character->set_last_location("/domain/Start/human/enter.c");
        } else {
            __Character->set_last_location("/domain/Nowhere/void.c");
        }
    }

    this_object()->shell_start();
    __Character->enter_world();

    if (newbie) {
        set_immortal(1); // @TODO FTUE
        // @TODO init_stats
        __Character->save_data();
        // @TODO init user?
    }
}

nomask private void character_takeover () {

}

nomask protected void character_exit () {
    if (__Character) {
        __Character->update_last_action();
        this_object()->query_account()->update_character_data(__Character->query_key_name(), query_character_summary()); // @TODO just pass __Character
        __Character->exit_world();
        __Character = 0;
        __BodyType = 0;
    }
}