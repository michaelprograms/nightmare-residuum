#ifndef __USER_H__
#define __USER_H__

/* --- user --- */
nomask void logon ();
nomask void net_dead ();
void receive_message (string type, string message);
void terminal_type (string term);
string query_terminal_type ();
string query_terminal_color();
nomask void quit_character (int destructing);
nomask void quit_account ();
nomask void reset_connect_timeout ();
nomask varargs void handle_remove (string message);

/* --- account --- */
object query_account ();
protected nomask varargs void account_input (int state, mixed extra, string input);
private void display_account_menu ();
#define STATE_ACCOUNT_ENTER                 0
#define STATE_ACCOUNT_HANDLE                1
#define STATE_ACCOUNT_CONFIRM               2
#define STATE_PASSWORD_NEW                  3
#define STATE_PASSWORD_CONFIRM              4
#define STATE_SCREENREADER_HANDLE           5
#define STATE_ACCOUNT_COMPLETE              6
#define STATE_ACCOUNT_PASSWORD              10
#define STATE_ACCOUNT_MENU                  11
#define STATE_CHARACTER_ENTER               20
#define STATE_CHARACTER_HANDLE              21
#define STATE_CHARACTER_CONFIRM_NAME        22
#define STATE_CHARACTER_DELETE              23
#define STATE_CHARACTER_DELETE_CONFIRM      24
#define STATE_PASSWORD_DELETE_CONFIRM       25
#define STATE_CHARACTER_OVERRIDE            26
#define STATE_CHARACTER_OVERRIDE_CONFIRM    27
#define STATE_SPECIES_ENTER                 30
#define STATE_SPECIES_HANDLE                31
#define STATE_SETTINGS_ENTER                40
#define STATE_SETTINGS_HANDLE               41
#define PROMPT_ACCOUNT_ENTER                "Enter account name: "
#define PROMPT_ACCOUNT_FORMAT               "Account names must be 4 to 64 characters containing alphanumerics, '_', '-', '.', or '@'."
#define PROMPT_ACCOUNT_CONFIRM              "Confirm account name: (y/N) "
#define PROMPT_PASSWORD_ENTER               "Password: "
#define PROMPT_PASSWORD_CREATE              "Create password: "
#define PROMPT_PASSWORD_CONFIRM             "Confirm password: "
#define PROMPT_SCREENREADER_ENTER           "Do you use a screenreader? (y/N) "
#define PROMPT_ACCOUNT_CHOICE               "Enter action: "
#define PROMPT_CHARACTER_FORMAT             "Character names must be 4 to 64 characters and must contain capitalized alphabetic characters, \"'\", \"-\", or spaces.\n"
#define PROMPT_CHARACTER_ENTER              "Enter new character name: "
#define PROMPT_CHARACTER_CONFIRM_NAME       "Confirm character name? (y/N) "
#define PROMPT_CHARACTER_DELETE             "Delete which character? "
#define PROMPT_CHARACTER_OVERRIDE           "Confirm connection override? (y/N) "
#define PROMPT_SPECIES_ENTER                "Enter character species: "
#define PROMPT_CHARACTER_DELETE_CONFIRM     "Enter character name to confirm deletion: "
#define PROMPT_PASSWORD_DELETE_CONFIRM      "Enter account password to confirm deletion: "
#define PROMPT_SETTINGS_ENTER               "Enter settings action: "

/* --- character --- */
private void set_character_name (string name);
private void set_character_species (string species);
string query_character_species ();
object query_character ();
int query_immortal ();
void set_immortal (int i);
nomask private void character_enter (int newbie);
nomask private void character_override ();
nomask void handle_character_override ();
nomask protected void character_exit ();
nomask protected void character_linkdead ();

/* --- input --- */
varargs nomask void input_push (function inputFn, mixed prompt, int secure, function callbackFn, int lock);
varargs nomask void input_single (function inputFn, mixed prompt, int secure, int lock);
varargs nomask void input_next (function inputFn, mixed prompt, int secure, int lock);
nomask void input_pop ();
nomask void input_focus ();

/* --- shell --- */
object query_shell ();
protected nomask void shell_start ();
protected nomask void shell_stop ();

/*  --- */

#endif /* __USER_H__ */
