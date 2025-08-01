// @this_object /std/user.c

#include "user.h"

#define QUERY_FIRST_IMMORTAL !sizeof(filter(get_dir("/realm/"), (: $1 && $1[0] != '.' :)))

nosave private string __Species;
/** @type {STD_CHARACTER} */
nosave private object __Character;

int valid_character () {
    return __Character && __Character->query_key_name() != 0 && __Character->query_species() != "unknown";
}

// -----------------------------------------------------------------------------

void set_character_name (string name) {
    if (!__Character) {
        __Character = new(STD_CHARACTER);
        __Character->set_user(this_object());
    }
    if (name) {
        __Character->set_name(name);
        __Character->set_account(this_object()->query_name());
    } else {
        destruct(__Character);
        __Character = new(STD_CHARACTER);
    }
}
void set_character_species (string species) {
    __Species = species;
    __Character->set_species(species);
}
string query_character_species () {
    return __Species;
}
/** @returns {STD_CHARACTER} */
object query_character () {
    return __Character;
}

// -----------------------------------------------------------------------------

nomask void character_enter (int newbie) {
    remove_call_out();

    __Character->setup_character(); // this calls restore_data/save_data
    if (newbie) {
        if (__Character->query_species() == "human") {
            __Character->set_environment_path("/domain/Origin/room/enter.c");
        } else {
            __Character->set_environment_path("/domain/Nowhere/room/void.c");
        }
    }

    this_object()->input_pop();
    this_object()->shell_start();
    __Character->enter_world();

    if (newbie) {
        if (QUERY_FIRST_IMMORTAL) {
            __Character->set_immortal(1);
            write("\n%^I_WHITE%^BOLD%^Since this appears to be the first connection, you have been set as immortal.%^RESET%^\n\n");
        }
        __Character->save_data();
    }

    __Character->set_last_action();
    this_object()->update_character_data(__Character);
}

nomask void character_reconnect (object char) {
    remove_call_out();

    if (!char) {
        error("Bad argument 1 to user/character->character_reconnect");
    }

    __Character = char;
    __Character->set_user(this_object());
    this_object()->shell_start();
    write("\n\nReturning " + __Character->query_cap_name() + " from linkdeath...\n\n");
    __Character->exit_freezer();
    __Character->set_last_action();
    this_object()->update_character_data(__Character);
}

/**
 * Handle overriding a character login. This account will transfer connection
 * of the character to this user.
 *
 * @param {STD_CHARACTER} character The character being overridden
 */
nomask void handle_character_override_new_connection (object character) {
    remove_call_out();

    if (!character) {
        error("Bad argument 1 to user/character->handle_character_override");
    }

    // Swap character's user
    __Character = character;
    __Character->set_user(this_object());

    this_object()->shell_start();
    __Character->enter_world(1);
    __Character->set_last_action();
    this_object()->update_character_data(__Character);
}

/**
 * Handle overriding a user's character from an old connection (this_object) to
 * a new connection.
 *
 * @param {STD_CHARACTER} character The character being overridden
 */
nomask void handle_character_override_old_connection (object character) {
    if (!character) {
        error("Bad argument 1 to user/character->handle_character_override_old_connection");
    }

    if (character == __Character) {
        __Character = 0;
        this_object()->handle_remove("\nYour connection has been overridden from " + query_ip_number(previous_object()) + ".\n\n");
    }
}

nomask void character_exit () {
    if (valid_character()) {
        __Character->exit_world();
        __Species = 0;
    }
}

nomask void character_linkdead () {
    if (valid_character()) {
        this_object()->update_character_data(__Character);
        __Character->enter_freezer();
    }
}
