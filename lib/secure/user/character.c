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
    // @TODO check if already have a name?
    if (name) {
        __Character->set_name(capitalize(name));
        // __Character->set_key_name(sanitize_name(name)); // @TODO this should be automatic when sanitize_name is a sefun
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

int query_immortal () { return __Immortal; }
void set_immortal (int i) {
    // @TODO security
    __Immortal = i;
}

// -----------------------------------------------------------------------------

nomask private void character_enter (int newbie) {
    object *chars, char;

    remove_call_out();

    // Check for existing character
    chars = filter_array(children(STD_CHARACTER) - ({ __Character }), (: $1 && $1->query_key_name() == __Character->query_key_name() && $1->query_user() :));
    if (sizeof(chars) > 0 && (char = chars[0])) {
        if (interactive(char->query_user())) {
            write(char->query_name()+" is connected and interactive.\n");
            account_input(STATE_CHARACTER_OVERRIDE);
            return;
        } else {
            destruct(__Character);
            __Character = char;
            __Character->set_user(this_object());
            shell_start();
            write("\n\nReturning " + __Character->query_name() + " from linkdeath...\n\n");
            __Character->exit_freezer();
            return;
        }
    } else { // fresh login
        __Character->setup_character(); // this calls restore_data/save_data

        if (newbie) {
            if (__Character->query_type() == "human") {
                __Character->set_last_location("/domain/Start/human/enter.c");
            } else {
                __Character->set_last_location("/domain/Nowhere/void.c");
            }
        }

        shell_start();
        __Character->enter_world();

        if (newbie) {
            set_immortal(1); // @TODO FTUE
            __Character->save_data();
        }
    }
}

nomask private void character_override () {
    object *chars, char;

    remove_call_out();

    // Verify existing character still here
    chars = filter_array(children(STD_CHARACTER) - ({ __Character }), (: $1 && $1->query_key_name() == __Character->query_key_name():));
    if (sizeof(chars) > 0 && (char = chars[0])) {
        destruct(__Character);
        // Notify and remove old user
        char->query_user()->handle_remove("\nYour connection has been overriden.\n\n");
        __Character = char;
        __Character->set_user(this_object());
        write("\n\nOverriding connection of " + __Character->query_name() + "...\n\n");
        shell_start();
        return;
    } else {
        write("\n"+__Character->query_key_name()+" no longer available to override.\n");
        character_enter(0);
        return;
    }
}

nomask protected void character_exit () {
    if (__Character) {
        __Character->update_last_action();
        query_account()->update_character_data(__Character);
        __Character->exit_world();
        __BodyType = 0;
    }
}

nomask protected void character_linkdead () {
    __Character->update_last_action();
    __Character->enter_freezer();
}