#include <config.h>

#include "user.h"

inherit M_SAVE;

private string __Name;
private string __Password;
private int __Created = time();
private int __LastOn = time();
private mapping __Characters = ([
    /* Data Format:
        STRING: ([                      // Name sanitized version
            "species":          STRING, // Species type for BODY
            "deleted":          INT,    // 0 or 1
            "level":            INT,
            "name":             STRING, // Name formatted version
            "last_action":      INT,    // last seen
            "last_location":    STRING  // last location short
        ]),
    */
]);
private mapping __Settings = ([ ]);
private int __EdSetup = 0;

// cache variable
nosave private string *__CharacterNames = ({ });

/* ----- ed setup ----- */

nomask void set_ed_setup (int code) {
    __EdSetup = code;
    save_data();
}

nomask int query_ed_setup () {
    return __EdSetup;
}

/* -----  ----- */

void ensure_default_settings () {
    // verify default settings exist
    if (undefinedp(__Settings["width"])) {
        __Settings["width"] = DEFAULT_WIDTH;
    }
    if (undefinedp(__Settings["lines"])) {
        __Settings["lines"] = DEFAULT_LINES;
    }
    if (undefinedp(__Settings["ansi"])) {
        __Settings["ansi"] = "on";
    }
    if (undefinedp(__Settings["screenreader"])) {
        __Settings["screenreader"] = "off";
    }
}

string query_name () {
    return __Name;
}
string query_key_name () {
    return __Name ? lower_case(__Name) : 0;
}

nomask void set_password (string str) {
    if (base_name(previous_object()) != STD_USER[0..<3]) {
        error("Illegal attempt to account->set_password");
    }
    __Password = str;
    save_data();
}
nomask string query_password () {
    return __Password;
}

int query_created () {
    return __Created;
}
int query_last_on () {
    return __LastOn;
}
void set_last_on () {
    __LastOn = time();
}

void add_character (string name, string nameClean, string species) {
    __Characters[nameClean] = ([
        "name": name,
        "deleted": 0,
        "species": species,
        "last_action": 0,
        "last_location": 0,
        "level": 0,
    ]);
    save_data();
}
mapping query_character_by_name (string name) {
    return copy(__Characters[name]);
}
int query_playable_characters () {
    mapping tmpCharacters = filter_mapping(__Characters, (: !$2["deleted"] :));
    __CharacterNames = sort_array(keys(tmpCharacters), 1);
    return sizeof(__CharacterNames) > 0;
}
string *query_character_names () {
    mapping tmpCharacters = filter_mapping(__Characters, (: !$2["deleted"] :));
    __CharacterNames = sort_array(keys(tmpCharacters), 1);
    return __CharacterNames;
}
void update_character_data (object character) {
    string name;
    if (!character || !(name = character->query_key_name())) {
        return;
    }
    __Characters[name]["species"] = character->query_species();
    __Characters[name]["level"] = character->query_level();
    __Characters[name]["last_action"] = character->query_last_action();
    __Characters[name]["last_location"] = character->query_environment_short();
    save_data();
}

void set_deleted (string name) {
    __Characters[name]["deleted"] = 1;
}

void set_setting (string key, mixed value) {
    if (!__Settings || !key) return;
    __Settings[key] = value;
    save_data();
}
mixed query_setting (string key) {
    if (!__Settings || !key) return 0;
    if (member_array(key, keys(__Settings)) == -1) return 0;
    return __Settings[key];
}
mapping query_settings () {
    return __Settings;
}

/* -----  ----- */

void set_name (string name) {
    if (!regexp(base_name(this_user()), "^("+STD_USER[0..<3]+"|"+D_TEST[0..<3]+")")) {
        return 0;
    }
    __Name = name;
    if (!stringp(__Name)) {  // name can be cleared during account creation
        return;
    }
    set_save_path(D_ACCOUNT->query_save_path(lower_case(__Name)));
    if (D_ACCOUNT->query_exists(__Name)) {
        restore_data();
    }

    ensure_default_settings();
}

/* -----  ----- */

private void display_account_menu () {
    string msg = "", characterMsg = "";

    if (!query_playable_characters()) {
        write("\nYou have no characters. You will now create a character.\n");
        account_input(STATE_CHARACTER_ENTER);
        return;
    }

    remove_call_out();
    msg = "\nAccount Actions   : " + format_syntax("settings") + " " + format_syntax("password") + " " + format_syntax("exit") + "%^RESET%^\nCharacter Actions : " + format_syntax("new") + " " + format_syntax("delete") + "\n\n";

    // @TODO different format for screenreader here?
    foreach (string name in query_character_names()) {
        mapping character = query_character_by_name(name);
        string tmp = "%^CYAN%^" + sprintf("%-22s", "<" + character["name"] + ">") + "%^RESET%^";
        tmp += sprintf("%-16s", capitalize(character["species"]+""));
        tmp += sprintf("%-24s", character["last_location"]);
        // @TODO change for connected / disconnected
        tmp += sprintf("%-20s", time_ago(character["last_action"]));
        tmp += "\n";
        characterMsg += tmp;
    }

    if (strlen(characterMsg) > 0) {
        msg += "Character           Species         Location                Last Seen\n" + characterMsg;
    }

    write(msg + "\n");
    input_next((: account_input, STATE_ACCOUNT_MENU, 0 :), PROMPT_ACCOUNT_CHOICE);
}

private string query_unlocked_species () {
    // string *unlocks; // @TODO
    //if (!sizeof(unlocks = explode(query_property("unlockedSpecies") || "", ","))) {
    //
    // } else {
    //     return "The following species are unlocked:\n" + implode(({"human"}) + unlocks, ", ") + "\n";
    // }

    return "The only species unlocked is human.\n";
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
                return handle_remove("\nInvalid entry. Connection terminated.\n");
            }
            if (!D_ACCOUNT->query_valid_name(input)) {
                write("\nThe account '"+input+"' is not a valid account name.\n");
                write(PROMPT_ACCOUNT_FORMAT + "\n");
                input_next((: account_input, STATE_ACCOUNT_HANDLE, ++extra :), PROMPT_ACCOUNT_ENTER);
                return;
            } else if (D_ACCOUNT->query_exists(input)) {
                write("\nExisting account '"+input+"'...\n");
                set_name(input);
                input_next((: account_input, STATE_ACCOUNT_PASSWORD, 0 :), PROMPT_PASSWORD_ENTER, 1);
            } else {
                reset_connect_timeout();
                set_name(input);
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
                set_name(0);
                if (extra >= 2) {
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
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
            set_password(extra);
            write("Password set.\n");
            input_next((: account_input, STATE_SCREENREADER_HANDLE, 0 :), PROMPT_SCREENREADER_ENTER);
            break;

        case STATE_SCREENREADER_HANDLE:
            if (input && strlen(input) > 0 && lower_case(input)[0..0] == "y") {
                set_setting("screenreader", "on");
                set_setting("ansi", "off");
            } else {
                set_setting("screenreader", "off");
            }
            write("Setting screenreader mode " + query_setting("screenreader") + ".\n");
            account_input(STATE_ACCOUNT_COMPLETE);
            break;

        case STATE_ACCOUNT_COMPLETE:
            D_LOG->log("account/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), query_name()));
            if (!query_playable_characters()) {
                write("\nWelcome, "+query_name()+"! You will now create a character.\n");
                account_input(STATE_CHARACTER_ENTER);
                return;
            } else {
                display_account_menu();
            }
            break;

        case STATE_ACCOUNT_PASSWORD:
            if (crypt(input, query_password()) == query_password()) {
                set_last_on();
                save_data();
                if (!query_playable_characters()) {
                    write("\nWelcome, "+query_name()+"! You will now create a character.\n");
                    account_input(STATE_CHARACTER_ENTER);
                } else {
                    write("\n\n%^BOLD%^Welcome back, "+query_name()+". Last seen "+time_ago(query_last_on())+".%^RESET%^\n");
                    display_account_menu();
                }
            } else {
                write("Invalid password.\n");
                if (extra >= 2) {
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
                input_next((: account_input, STATE_ACCOUNT_PASSWORD, ++extra :), PROMPT_PASSWORD_ENTER, 1);
            }
            break;
        case STATE_ACCOUNT_MENU:
            if (!input || input == "" || !(input = lower_case(input))) {
                display_account_menu();
            } else if (input == "exit") {
                set_last_on();
                save_data();
                handle_remove("\nExiting account. Connection closed.\n");
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
            } else if (member_array(input, query_character_names()) > -1) {
                // Check for existing character
                if (extra = find_character(input)) {
                    if (extra->query_user() && interactive(extra->query_user())) {
                        reset_connect_timeout();
                        write(extra->query_cap_name()+" is connected and interactive.\n\n");
                        input_next((: account_input, STATE_CHARACTER_OVERRIDE, extra :), PROMPT_CHARACTER_OVERRIDE);
                    } else {
                        character_reconnect(extra);
                    }
                } else { // fresh login
                    set_character_name(input);
                    character_enter(0);
                }
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
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
            } else {
                if (input && input == "exit") {
                    set_last_on();
                    save_data();
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
                    return handle_remove("\nInvalid entry. Connection terminated.\n");
                }
                write("\nCanceled new character.\n");
                input_next((: account_input, STATE_CHARACTER_HANDLE, ++extra :), PROMPT_CHARACTER_ENTER);
                return;
            }
            reset_connect_timeout();
            account_input(STATE_SPECIES_ENTER);
            break;

        case STATE_CHARACTER_DELETE:
            if (!input || input == "" || !(input = lower_case(input))) {
                return display_account_menu();
            }
            // @TODO member_array(input, D_ACCESS->query_secure()) { write("Cannot delete"); }
            input = sanitize_name(input);
            if (member_array(input, query_character_names()) == -1) {
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
            if (crypt(input, query_password()) == query_password()) {
                write("\nDeleted character '" + extra + "'.\n");
                set_deleted(extra);
                set_last_on();
                save_data();
            } else {
                write("Invalid password. Character deletion cancelled.\n");
            }
            display_account_menu();
            break;

        case STATE_CHARACTER_OVERRIDE:
            if ((input = lower_case(input)) == "" || input[0..0] != "y") {
                write("\nCanceled character connection override.\n");
                account_input(STATE_ACCOUNT_MENU);
                return;
            } else {
                write("extra: "+identify(extra)+"\n");
                write("\n\nOverriding connection of " + extra->query_cap_name() + "...\n\n");
                if (extra->query_user()) {
                    extra->query_user()->handle_character_override();
                }
                character_override(extra);
            }
            break;

        case STATE_SPECIES_ENTER:
            write("Characters must have a species.\n\n");
            write(query_unlocked_species() + "\n");
            input_next((: account_input, STATE_SPECIES_HANDLE, 0 :), PROMPT_SPECIES_ENTER);
            break;

        case STATE_SPECIES_HANDLE:
            if (input != "human") {
                write("\nThe only one type option available to your account is human.\n");
                input_next((: account_input, STATE_SPECIES_HANDLE, 0 :), PROMPT_SPECIES_ENTER);
                return;
            }
            write(query_character()->query_cap_name() + " is a " + input + "!\n\n");
            set_character_species(input);
            add_character(query_character()->query_cap_name(), query_character()->query_key_name(), query_character()->query_species());
            D_LOG->log("character/new", sprintf("%s : %s : %s\n", ctime(time()), query_ip_number(), input));
            write("Entering as " + query_character()->query_cap_name() + "...\n");
            character_enter(1);
            break;

        case STATE_SETTINGS_ENTER:
            write("Settings Actions  : " + format_syntax("back") + " " + format_syntax("[setting] [option]") + "\n\n");
            foreach (string setting in sort_array(keys(query_settings()), 1)) {
                string display;
                if (intp(query_setting(setting))) {
                    display = "" + query_setting(setting);
                } else {
                    display = query_setting(setting);  // @TODO ?
                }
                write("  " + sprintf("%-24s", setting) + "  " + display + "\n");
            }
            write("\n");
            input_next((: account_input, STATE_SETTINGS_HANDLE, 0 :), PROMPT_SETTINGS_ENTER);
            break;

        case STATE_SETTINGS_HANDLE:
            if (input && input != "") {
                string *settings = keys(query_settings());
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
                    if (intp(query_setting(setting))) {
                        int w = to_int(input);
                        if (setting == "width") {
                            if (w < 40) w = 40;
                        }
                        set_setting(setting, w);
                        write("Setting " + setting + " mode to " + w + ".\n");
                    } else {
                        if (member_array(input, ({ "on", "off"})) > -1) {
                            set_setting(setting, input);
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

void create () {
    ensure_default_settings();
}