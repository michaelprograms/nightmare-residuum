#include "user.h"

nosave private object __Account;

object query_account () { return __Account; }

// -----------------------------------------------------------------------------

protected void display_account_menu () {
    string msg = "", characterMsg = ""; // creatorMsg = "",

    if (!__Account->query_playable_characters()) {
        write("\nYou have no characters. You will now create a character.\n");
        account_input(STATE_CHARACTER_ENTER);
        return;
    }

    reset_connect_timeout(1);
    msg = "\nAccount Actions   : %^CYAN%^[settings] [password] [exit]%^RESET%^\nCharacter Actions : %^CYAN%^[new] [delete]%^RESET%^\n\n";

    // @TODO different format for screenreader here?
    foreach (string name in __Account->query_character_names()) {
        mapping character = __Account->query_character(name);
        string tmp = "%^CYAN%^" + sprintf("%-20s", "["+character["name"]+"]") + "%^RESET%^";
        tmp += sprintf("%-16s", capitalize(character["type"]+""));
        tmp += sprintf("%-24s", character["last_location"]);
        // @TODO change for connected / disconnected
        tmp += sprintf("%-20s", time_ago(character["last_action"]));
        tmp += "\n";
        characterMsg += tmp;
    }

    if (sizeof(characterMsg) > 0) {
        msg += "Character           Species         Location                Last Seen\n" + characterMsg;
    }

    write(msg + "\n");
    input_next((: account_input(STATE_ACCOUNT_MENU, 0) :), PROMPT_ACCOUNT_CHOICE);
}

private string query_unlocked_species () {
    string *unlocks;
    if (!sizeof(unlocks = explode(__Account->query_property("unlockedSpecies") || "", ","))) {
        return "The only species unlocked is human.\n";
    } else {
        return "The following species are unlocked:\n" + implode(({"human"}) + unlocks, ", ") + "\n";
    }
}

protected varargs void account_input (int state, mixed extra, string input) {
    switch (state) {
        case STATE_ACCOUNT_ENTER:
            reset_connect_timeout();
            input_push((: account_input(STATE_ACCOUNT_HANDLE, 0) :), PROMPT_ACCOUNT_ENTER);
            input_focus();
            break;

        case STATE_ACCOUNT_HANDLE:
            if (!input || input == "") {
                return handle_remove("\nInvalid entry. Connection terminated.\n");
            }
            if (!D_ACCOUNT->query_valid_name(input)) {
                write("\nThe account '"+input+"' is not a valid account name.\n");
                write(PROMPT_ACCOUNT_FORMAT + "\n");
                input_next((: account_input(STATE_ACCOUNT_HANDLE, ++extra) :), PROMPT_ACCOUNT_ENTER);
                return;
            } else if (D_ACCOUNT->query_exists(input)) {
                write("\nExisting account '"+input+"'...\n");
                __Account = clone_object(STD_ACCOUNT);
                __Account->set_name(input);
                input_next((: account_input(STATE_ACCOUNT_PASSWORD, 0) :), PROMPT_PASSWORD_ENTER, 1);
            } else {
                reset_connect_timeout();
                __Account = clone_object(STD_ACCOUNT);
                __Account->set_name(input);
                write("\nNew account '"+input+"'!\n");
                write("You should pick a sensible and unique account name.\n");
                write(PROMPT_ACCOUNT_FORMAT + "\n");
                write("Accounts can create numerous characters.\n");
                input_next((: account_input(STATE_ACCOUNT_CONFIRM, extra) :), PROMPT_ACCOUNT_CONFIRM);
                return;
            }
            reset_connect_timeout();
            break;

        case STATE_ACCOUNT_CONFIRM:
            if ((input = lower_case(input)) == "" || input[0..0] != "y") {
                __Account->set_name(0);
                if (extra >= 2) {
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
                write("\nCanceled new account.\n");
                input_next((: account_input(STATE_ACCOUNT_HANDLE, ++extra) :), PROMPT_ACCOUNT_ENTER);
                return;
            }
            reset_connect_timeout();
            write("Passwords must be at least 8 characters long.\n");
            input_next((: account_input(STATE_PASSWORD_NEW, 0) :), PROMPT_PASSWORD_CREATE, 1);
            break;

        case STATE_PASSWORD_NEW:
            if (sizeof(input) < 8) {
                write("\nPassword did not meet requirements.\n\n");
                return;
            }
            reset_connect_timeout();
            input_next((: account_input(STATE_PASSWORD_CONFIRM, crypt(input, 0)) :), PROMPT_PASSWORD_CONFIRM, 1);
            break;

        case STATE_PASSWORD_CONFIRM:
            if (crypt(input, extra) != extra) {
                write("\nPassword entries do not match.\n");
                input_next((: account_input(STATE_PASSWORD_NEW, 0) :), PROMPT_PASSWORD_CREATE, 1);
                return;
            }
            __Account->set_password(extra);
            write("Password set.\n");
            input_next((: account_input(STATE_SCREENREADER_HANDLE, 0) :), PROMPT_SCREENREADER_ENTER);
            break;

        case STATE_SCREENREADER_HANDLE:
            if (input && sizeof(input) > 0 && lower_case(input)[0..0] == "y") {
                __Account->set_setting("screenreader", "on");
                __Account->set_setting("ansi", "off");
            } else {
                __Account->set_setting("screenreader", "off");
            }
            write("Setting screenreader mode " + __Account->query_setting("screenreader") + ".\n");
            account_input(STATE_ACCOUNT_COMPLETE);
            break;

        case STATE_ACCOUNT_COMPLETE:
            // D_LOG->log("account/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), __Account->query_name()));
            if (!__Account->query_playable_characters()) {
                write("\nWelcome, "+__Account->query_name()+"! You will now create a character.\n");
                account_input(STATE_CHARACTER_ENTER);
                return;
            } else {
                display_account_menu();
            }
            break;

        case STATE_ACCOUNT_PASSWORD:
            if (crypt(input, __Account->query_password()) == __Account->query_password()) {
                __Account->set_last_on();
                __Account->save_data();
                if (!__Account->query_playable_characters()) {
                    write("\nWelcome, "+__Account->query_name()+"! You will now create a character.\n");
                    account_input(STATE_CHARACTER_ENTER);
                } else {
                    write("\n\n%^BOLD%^Welcome back, "+__Account->query_name()+". Last seen "+time_ago(__Account->query_last_on())+".%^RESET%^\n");
                    display_account_menu();
                }
            } else {
                write("Invalid password.\n");
                if (extra >= 2) {
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
                input_next((: account_input(STATE_ACCOUNT_PASSWORD, ++extra) :), PROMPT_PASSWORD_ENTER, 1);
            }
            break;
        case STATE_ACCOUNT_MENU:
            if (!input || input == "" || !(input = lower_case(input))) {
                display_account_menu();
            } else if (input == "exit") {
                __Account->set_last_on();
                __Account->save_data();
                handle_remove("\nExiting account. Connection closed.\n");
            } else if (input == "settings") {
                write("\nAccount Settings\n\n");
                account_input(STATE_SETTINGS_ENTER);
            } else if (input == "passwd") {
                reset_connect_timeout();
                write("Changing password...\n");
                input_next((: account_input(STATE_PASSWORD_NEW, 0) :), PROMPT_PASSWORD_CREATE, 1);
            } else if (input == "new") {
                account_input(STATE_CHARACTER_ENTER);
            } else if (input == "delete") {
                input_next((: account_input(STATE_CHARACTER_DELETE, 0) :), PROMPT_CHARACTER_DELETE);
            } else if (member(input, __Account->query_character_names()) > -1) {
                set_character_name(input);
                character_enter(0);
            } else {
                write("Invalid input choice received.\n\n");
                display_account_menu();
            }
            break;

        case STATE_CHARACTER_ENTER:
            reset_connect_timeout();
            write(PROMPT_CHARACTER_FORMAT);
            input_next((: account_input(STATE_CHARACTER_HANDLE, 0) :), PROMPT_CHARACTER_ENTER);
            break;

        case STATE_CHARACTER_HANDLE:
            if (!input || input == "") {
                if (extra >= 2) {
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
            } else {
                if (input && input == "exit") {
                    __Account->set_last_on();
                    __Account->save_data();
                    handle_remove("\nExiting account. Connection closed.\n");
                }
                if (!D_CHARACTER->query_valid_name(input)) {
                    write("\nThe character '"+input+"' is not a valid name for new characters.\n");
                    write(PROMPT_CHARACTER_FORMAT);
                } else if (D_CHARACTER->query_exists(input)) {
                    write("\nThe character '"+input+"' exists already.\n");
                    write(PROMPT_CHARACTER_FORMAT);
                } else {
                    reset_connect_timeout();
                    set_character_name(input);
                    write("\nNew character '" + input + "'!\n");
                    input_next((: account_input(STATE_CHARACTER_CONFIRM_NAME, ++extra) :), PROMPT_CHARACTER_CONFIRM_NAME);
                    return;
                }
            }
            input_next((: account_input(STATE_CHARACTER_HANDLE, ++extra) :), PROMPT_CHARACTER_ENTER);
            break;

        case STATE_CHARACTER_CONFIRM_NAME:
            if ((input = lower_case(input)) == "" || input[0..0] != "y") {
                set_character_name(0);
                if (extra >= 2) {
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
                write("\nCanceled new character.\n");
                input_next((: account_input(STATE_CHARACTER_HANDLE, ++extra) :), PROMPT_CHARACTER_ENTER);
                return;
            }
            reset_connect_timeout();
            account_input(STATE_SPECIES_ENTER);
            break;

        case STATE_CHARACTER_DELETE:
            if (!input || input == "" || !(input = lower_case(input))) {
                return display_account_menu();
            }
            // @TODO member(input, D_ACCESS->query_secure()) { write("Cannot delete"); }
            input = sanitize_name(input);
            if (member(input, __Account->query_character_names()) == -1) {
                write("Invalid character name.\n\n");
                display_account_menu();
                return;
            }
            input_next((: account_input(STATE_CHARACTER_DELETE_CONFIRM, input) :), PROMPT_CHARACTER_DELETE_CONFIRM + "[" + input + "] ");
            break;

        case STATE_CHARACTER_DELETE_CONFIRM:
            if (input == extra) {
                input_next((: account_input(STATE_PASSWORD_DELETE_CONFIRM, input) :), PROMPT_PASSWORD_DELETE_CONFIRM);
                return;
            }
            write("Character name not confirmed. Delete cancelled.\n");
            display_account_menu();
            break;

        case STATE_PASSWORD_DELETE_CONFIRM:
            if (crypt(input, __Account->query_password()) == __Account->query_password()) {
                write("\nDeleted character '" + extra + "'.\n");
                __Account->set_deleted(extra);
                __Account->set_last_on();
                __Account->save_data();
            } else {
                write("Invalid password. Character deletion cancelled.\n");
            }
            display_account_menu();
            break;

        case STATE_CHARACTER_OVERRIDE:
            reset_connect_timeout();
            input_next((: account_input(STATE_CHARACTER_OVERRIDE_CONFIRM, input) :), PROMPT_CHARACTER_OVERRIDE);
            break;

        case STATE_CHARACTER_OVERRIDE_CONFIRM:
            if ((input = lower_case(input)) == "" || input[0..0] != "y") {
                write("\nCanceled character connection override.\n");
                return;
            }
            character_override();
            break;

        case STATE_SPECIES_ENTER:
            write("Characters must have a species.\n\n");
            write(query_unlocked_species() + "\n");
            input_next((: account_input(STATE_SPECIES_HANDLE, 0) :), PROMPT_SPECIES_ENTER);
            break;

        case STATE_SPECIES_HANDLE:
            if (input != "human") {
                write("\nThe only one type option available to your account is human.\n");
                input_next((: account_input(STATE_SPECIES_HANDLE, 0) :), PROMPT_SPECIES_ENTER);
                return;
            }
            write(query_character()->query_cap_name() + " is a " + input + "!\n\n");
            set_character_species(input);
            __Account->add_character(query_character()->query_cap_name(), query_character()->query_key_name(), query_character()->query_species());
            // D_LOG->log("character/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), input));
            write("Entering as " + query_character()->query_cap_name() + "...\n");
            character_enter(1);
            break;

        case STATE_SETTINGS_ENTER:
            write("Settings Actions  : %^CYAN%^[back] [(setting) (option)]%^RESET%^\n\n");
            foreach (string setting in sort_array(m_indices(__Account->query_settings()), (: $1 > $2 :))) {
                string display;
                if (intp(__Account->query_setting(setting))) {
                    display = "" + __Account->query_setting(setting);
                } else {
                    display = __Account->query_setting(setting);  // @TODO ?
                }
                write("  " + sprintf("%-24s", setting) + "  " + display + "\n");
            }
            write("\n");
            input_next((: account_input(STATE_SETTINGS_HANDLE, 0) :), PROMPT_SETTINGS_ENTER);
            break;

        case STATE_SETTINGS_HANDLE:
            if (input && input != "") {
                string *settings = keys(__Account->query_settings());
                string *split = explode(input, " ");
                string setting = split[0];
                if (setting == "back") {
                    write("\n");
                    account_input(STATE_ACCOUNT_MENU);
                    return;
                }
                input = sizeof(split) > 1 ? input[(sizeof(setting)+1)..] : 0;
                if (member(setting, settings) == -1) {
                    write("Invalid setting.\n");
                } else {
                    if (intp(__Account->query_setting(setting))) {
                        int w = to_int(input);
                        if (setting == "width") {
                            if (w < 40) w = 40;
                        }
                        __Account->set_setting(setting, w);
                        write("Setting " + setting + " mode to " + w + ".\n");
                    } else {
                        if (member(input, ({ "on", "off"})) > -1) {
                            __Account->set_setting(setting, input);
                            write("Setting " + setting + " mode " + input + ".\n");
                        } else {
                            write("Invalid setting.\n");
                        }
                    }
                }
            }
            account_input(STATE_SETTINGS_ENTER);
            break;
    }
}