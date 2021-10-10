#include "user.h"

nosave private object __Account = new("/std/account.c");

object query_account () { return __Account; }

// -----------------------------------------------------------------------------

private void display_account_menu () {
    string msg = "", characterMsg = ""; // creatorMsg = "",

    if (!__Account->query_has_playable_characters()) {
        write("\nYou have no characters. You will now create a character.\n");
        account_input(STATE_CHARACTER_ENTER);
        return;
    }

    remove_call_out();
    msg = "\nAccount Actions   : %^CYAN%^[settings] [password] [exit]%^RESET%^\nCharacter Actions : %^CYAN%^[new] [delete]%^RESET%^\n\n";

    // @TODO different format for screenreader here?
    foreach(string name in __Account->query_character_names()) {
        mapping character = __Account->query_character(name);
        string tmp = "%^CYAN%^" + pad_right("[" + character["name"] + "]", 20) + "%^RESET%^";
        tmp += pad_right(capitalize(character["type"]+""), 20);
        tmp += pad_right(character["last_location"], 20);
        // @TODO change for connected / disconnected
        tmp += pad_right(time_ago(character["last_action"]), 20);
        tmp += "\n";
        characterMsg += tmp;
    }

    if (strlen(characterMsg) > 0) {
        msg += "Character           Type                Location            Last Seen\n" + characterMsg;
    }

    write(msg + "\n");
    input_next((: account_input, STATE_ACCOUNT_MENU, 0 :), PROMPT_ACCOUNT_CHOICE);
}

protected nomask varargs void account_input (int state, mixed extra, string input) {
    switch (state) {
        case STATE_ACCOUNT_ENTER:
            reset_connect_timeout();
            input_push((: account_input, STATE_ACCOUNT_HANDLE, 0 :), PROMPT_ACCOUNT_ENTER);
            input_focus();
            break;

        case STATE_ACCOUNT_HANDLE:
            if (!input || input == "") {
                return handle_destruct("\nInvalid entry. Connection terminated.\n");
            }
            if (this_object()->handle_login_commands(input)) return; // @TODO clean this up
            if (!D_ACCOUNT->query_valid_name(input)) {
                write("\nThe account '"+input+"' is not a valid account name.\n");
                write(PROMPT_ACCOUNT_FORMAT + "\n");
                input_next((: account_input, STATE_ACCOUNT_HANDLE, ++extra :), PROMPT_ACCOUNT_ENTER);
                return;
            } else if (D_ACCOUNT->query_exists(input)) {
                write("\nExisting account '"+input+"'...\n");
                __Account->set_name(input);
                input_next((: account_input, STATE_ACCOUNT_PASSWORD, 0 :), PROMPT_PASSWORD_ENTER, 1);
            } else {
                reset_connect_timeout();
                __Account->set_name(input);
                write("\nNew account '"+input+"'!\n");
                write("You should pick a sensible and unique account name.\n");
                write(PROMPT_ACCOUNT_FORMAT + "\n");
                write("Accounts can create numerous characters.\n");
                input_next((: account_input, STATE_ACCOUNT_CONFIRM, extra :), PROMPT_ACCOUNT_CONFIRM);
                return;
            }
            reset_connect_timeout();
            break;

        case STATE_ACCOUNT_CONFIRM:
            if ((input = lower_case(input)) == "" || input[0..0] != "y") {
                __Account->set_name(0);
                if (extra >= 2) {
                    return handle_destruct("\nInvalid entry. Connection terminated.\n");
                }
                write("\nCanceled new account.\n");
                input_next((: account_input, STATE_ACCOUNT_HANDLE, ++extra :), PROMPT_ACCOUNT_ENTER);
                return;
            }
            reset_connect_timeout();
            write("Passwords must be at least 8 characters long.\n");
            input_next((: account_input, STATE_PASSWORD_NEW, 0 :), PROMPT_PASSWORD_CREATE, 1);
            break;

        case STATE_PASSWORD_NEW:
            if (strlen(input) < 8) {
                write("\nPassword did not meet requirements.\n\n");
                return;
            }
            reset_connect_timeout();
            input_next((: account_input, STATE_PASSWORD_CONFIRM, crypt(input, 0) :), PROMPT_PASSWORD_CONFIRM, 1);
            break;

        case STATE_PASSWORD_CONFIRM:
            if (crypt(input, extra) != extra) {
                write("\nPassword entries do not match.\n");
                input_next((: account_input, STATE_PASSWORD_NEW, 0 :), PROMPT_PASSWORD_CREATE, 1);
                return;
            }
            __Account->set_password(extra);
            write("Password set.\n");
            input_next((: account_input, STATE_SCREENREADER_HANDLE, 0 :), PROMPT_SCREENREADER_ENTER);
            break;

        case STATE_SCREENREADER_HANDLE:
            if (input && strlen(input) > 0 && lower_case(input)[0..0] == "y") {
                __Account->set_setting("screenreader", "on");
                __Account->set_setting("ansi", "off");
            } else {
                __Account->set_setting("screenreader", "off");
            }
            write("Setting screenreader mode " + __Account->query_setting("screenreader") + ".\n");
            if (__Account->query_setting("screenreader") == "off") input_next((: account_input, STATE_ANSI_HANDLE, 0 :), PROMPT_ANSI_PROMPT);
            else account_input(STATE_ACCOUNT_COMPLETE);
            break;

        case STATE_ANSI_HANDLE:
            if (input && strlen(input) > 0 && lower_case(input)[0..0] == "y") {
                __Account->set_setting("ansi", "on");
            }
            write("Setting ansi mode " + __Account->query_setting("ansi") + ".\n");
            account_input(STATE_ACCOUNT_COMPLETE);
            break;

        case STATE_ACCOUNT_COMPLETE:
            // @TODO log_file("account/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), __Account->query_name()));
            if (!__Account->query_has_playable_characters()) {
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
                if (member_array("screenreader", keys(__Account->query_settings())) == -1) {
                    input_next((: account_input, STATE_SCREENREADER_HANDLE, 0 :), PROMPT_SCREENREADER_ENTER);
                } else if (__Account->query_setting("screenreader") == "off" && member_array("ansi", keys(__Account->query_settings())) == -1) {
                    input_next((: account_input, STATE_ANSI_HANDLE, 0 :), PROMPT_ANSI_PROMPT);
                } else if (!__Account->query_has_playable_characters()) {
                    write("\nWelcome, "+__Account->query_name()+"! You will now create a character.\n");
                    account_input(STATE_CHARACTER_ENTER);
                } else {
                    write("\n\n%^BOLD%^Welcome back, "+__Account->query_name()+". Last seen "+time_ago(__Account->query_last_on())+".%^RESET%^\n");
                    display_account_menu();
                }
            } else {
                write("Invalid password.\n");
                if (extra >= 2) {
                    return handle_destruct("\nInvalid entry. Connection terminated.\n");
                }
                input_next((: account_input, STATE_ACCOUNT_PASSWORD, ++extra :), PROMPT_PASSWORD_ENTER, 1);
            }
            break;
        case STATE_ACCOUNT_MENU:
            if (!input || input == "" || !(input = lower_case(input))) {
                display_account_menu();
            } else if (input == "exit") {
                __Account->set_last_on();
                handle_destruct("\nExiting account. Connection closed.\n");
            } else if (input == "settings") {
                write("\nAccount Settings\n\n");
                account_input(STATE_SETTINGS_ENTER);
            } else if (input == "passwd") {
                reset_connect_timeout();
                write("Changing password...\n");
                input_next((: account_input, STATE_PASSWORD_NEW, 0 :), PROMPT_PASSWORD_CREATE, 1);
            } else if (input == "new") {
                account_input(STATE_CHARACTER_ENTER);
            } else if (input == "delete") {
                input_next((: account_input, STATE_CHARACTER_DELETE, 0 :), PROMPT_CHARACTER_DELETE);
            } else if (member_array(input, __Account->query_character_names()) > -1) {


                write("\n\nEntering as " + query_character()->query_name() + "...\n\n");
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
            input_next((: account_input, STATE_CHARACTER_HANDLE, 0 :), PROMPT_CHARACTER_ENTER);
            break;

        case STATE_CHARACTER_HANDLE:
            if (!input || input == "") {
                if (extra >= 2) {
                    return handle_destruct("\nInvalid entry. Connection terminated.\n");
                }
            } else {
                if (input && input == "exit") {
                    __Account->set_last_on();
                    handle_destruct("\nExiting account. Connection closed.\n");
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
                    input_next((: account_input, STATE_CHARACTER_CONFIRM_NAME, ++extra :), PROMPT_CHARACTER_CONFIRM_NAME);
                    return;
                }
            }
            input_next((: account_input, STATE_CHARACTER_HANDLE, ++extra :), PROMPT_CHARACTER_ENTER);
            break;

        case STATE_CHARACTER_CONFIRM_NAME:
            if ((input = lower_case(input)) == "" || input[0..0] != "y") {
                set_character_name(0);
                if (extra >= 2) {
                    return handle_destruct("\nInvalid entry. Connection terminated.\n");
                }
                write("\nCanceled new character.\n");
                input_next((: account_input, STATE_CHARACTER_HANDLE, ++extra :), PROMPT_CHARACTER_ENTER);
                return;
            }
            reset_connect_timeout();
            // account_input(STATE_TYPE_ENTER);
            __Account->add_character(query_character()->query_name(), query_character()->query_key_name(), 0);
            write("Entering as " + query_character()->query_name() + "...\n");

            // @TODO log_file("character/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), input));

            character_enter(1);
            break;

        case STATE_CHARACTER_DELETE:
            if (!input || input == "" || !(input = lower_case(input))) {
                return display_account_menu();
            }
            // @TODO member_array(input, D_ACCESS->query_secure()) { write("Cannot delete"); }
            input = sanitize_name(input);
            if (member_array(input, __Account->query_character_names()) == -1) {
                write("Invalid character name.\n\n");
                display_account_menu();
                return;
            }
            input_next((: account_input, STATE_CHARACTER_DELETE_CONFIRM, input :), PROMPT_CHARACTER_DELETE_CONFIRM + "[" + input + "] ");
            break;

        case STATE_CHARACTER_DELETE_CONFIRM:
            if (input == extra) {
                input_next((: account_input, STATE_PASSWORD_DELETE_CONFIRM, input :), PROMPT_PASSWORD_DELETE_CONFIRM);
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
            } else {
                write("Invalid password. Character deletion cancelled.\n");
            }
            display_account_menu();
            break;

        // case STATE_TYPE_ENTER:
        //     write("Characters must select a type.\n");
        //     // @TODO check account properties for unlocks
        //     write("\nThe only one type option available to your account is human.\n");
        //     input_next((: account_input, STATE_TYPE_HANDLE, 0 :), PROMPT_TYPE_ENTER);
        //     break;

        // case STATE_TYPE_HANDLE:
        //     if (input != "human") {
        //         write("\nThe only one type option available to your account is human.\n");
        //         input_next((: account_input, STATE_TYPE_HANDLE, 0 :), PROMPT_TYPE_ENTER);
        //         return;
        //     }
        //     write(query_character_name() + " is now a " + input + "...\n");
        //     set_character_type(input);
        //     __Account->add_character(query_character()->query_name(), query_character()->query_key_name(), query_character_type());
        //     write("Entering as " + query_character_name() + "...\n");

        //     // @TODO log_file("character/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), input));

        //     character_enter(1);
        //     break;

        case STATE_SETTINGS_ENTER:
            write("Settings Actions  : %^CYAN%^[back] [(setting) (option)]%^RESET%^\n\n");
            foreach(string setting in sort_array(keys(__Account->query_settings()), 1)) {
                string display;
                if (intp(__Account->query_setting(setting))) {
                    display = "" + __Account->query_setting(setting);
                } else {
                    display = __Account->query_setting(setting);  // @TODO ?
                }
                write("  " + pad_right(setting, 24) + "  " + display + "\n");
            }
            write("\n");
            input_next((: account_input, STATE_SETTINGS_HANDLE, 0 :), PROMPT_SETTINGS_ENTER);
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
                input = sizeof(split) > 1 ? input[(strlen(setting)+1)..] : 0;
                if (member_array(setting, settings) == -1) {
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
                        if (member_array(input, ({ "on", "off"})) > -1) {
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