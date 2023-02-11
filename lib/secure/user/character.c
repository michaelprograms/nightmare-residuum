#include "user.h"

#define QUERY_FIRST_IMMORTAL !!filter(get_dir("/realm/"), (:$1 && $1[0..0] != ".":))

nosave private string __Species;
nosave private object __Character;

// -----------------------------------------------------------------------------

private void set_character_name (string name) {
    if (!__Character) {
        __Character = clone_object(STD_CHARACTER);
        __Character->set_user(this_object());
    }
    // @TODO check if already have a name?
    if (name) {
        __Character->set_name(name);
        // __Character->set_key_name(sanitize_name(name)); // @TODO this should be automatic when sanitize_name is a sefun
    } else {
        destruct(__Character);
        __Character = clone_object(STD_CHARACTER);
    }
}
private void set_character_species (string species) {
    __Species = species;
    __Character->set_species(species);
}
string query_character_species () {
    return __Species;
}
object query_character () {
    return __Character;
}

// -----------------------------------------------------------------------------

nomask private void character_enter (int newbie) {
    object *chars, char;

    remove_call_out();

    // Check for existing character
    chars = filter(children(STD_CHARACTER) - ({ __Character }), (: $1 && $1->query_key_name() == __Character->query_key_name() :));
    if (sizeof(chars) > 0 && (char = chars[0])) {
        if (char->query_user() && interactive(char->query_user())) {
            write(char->query_cap_name()+" is connected and interactive.\n");
            account_input(STATE_CHARACTER_OVERRIDE);
            return;
        } else {
            destruct(__Character);
            __Character = char;
            __Character->set_user(this_object());
            shell_start();
            write("\n\nReturning " + __Character->query_cap_name() + " from linkdeath...\n\n");
            __Character->exit_freezer();
            return;
        }
    } else { // fresh login
        __Character->setup_character(); // this calls restore_data/save_data
        if (newbie) {
            if (__Character->query_species() == "human") {
                __Character->set_environment_path("/domain/Start/human/room/enter.c");
            } else {
                __Character->set_environment_path("/domain/Nowhere/room/void.c");
            }
        }

        shell_start();
        __Character->enter_world();

        if (newbie) {
            if (QUERY_FIRST_IMMORTAL) {
                __Character->set_immortal(1);
                write("\n%^BOLD%^Since this appears to be the first connection, you have been set as immortal.%^RESET%^\n\n");
            }
            __Character->save_data();
        }
    }
}

nomask private void character_override () {
    object *chars, char;

    remove_call_out();

    // Verify existing character still here
    chars = filter(children(STD_CHARACTER) - ({ __Character }), (: $1 && $1->query_key_name() == __Character->query_key_name():));
    if (sizeof(chars) > 0 && (char = chars[0])) {
        destruct(__Character);
        // Swap character's user
        __Character = char;
        __Character->query_user()->handle_character_override();
        __Character->set_user(this_object());
        __Character->set_parent(this_object());
        write("\n\nOverriding connection of " + __Character->query_cap_name() + "...\n\n");
        shell_start();
        __Character->enter_world(1);
        return;
    } else {
        write("\n"+__Character->query_cap_name()+" no longer available to override.\n");
        character_enter(0);
        return;
    }
}

nomask void handle_character_override () {
    object po;
    if (base_name(po = previous_object()) == OBJ_USER && po->query_character() == __Character) {
        __Character = 0;
        handle_remove("\nYour connection has been overriden from " + query_ip_number(po) + ".\n\n");
    }
}

nomask protected void character_exit () {
    if (__Character) {
        __Character->update_last_action();
        query_account()->update_character_data(__Character);
        __Character->exit_world();
        __Species = 0;
    }
}

nomask protected void character_linkdead () {
    __Character->update_last_action();
    __Character->enter_freezer();
}
