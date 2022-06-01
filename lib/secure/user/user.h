#ifndef __USER_H__
#define __USER_H__

/* --- user --- */
// nomask void logon ();
// nomask void net_dead ();
void receive_message (string type, string message);
void terminal_type (string term);
string query_terminal_type ();
string query_terminal_color();
void quit_character (int destructing);
void quit_account ();
varargs void reset_connect_timeout (int stop);
varargs void handle_remove (string message);

/* --- account --- */
object query_account ();
protected varargs void account_input (int state, mixed extra, string input);
protected void display_account_menu ();
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
protected void set_character_name (string name);
protected void set_character_species (string species);
string query_character_species ();
object query_character ();
void set_immortal (int i);
protected void character_enter (int newbie);
protected void character_override ();
void handle_character_override ();
protected void character_exit ();
protected void character_linkdead ();

/* --- input --- */
varargs void input_push (closure inputFn, mixed prompt, int secure, closure callbackFn, int lock);
varargs void input_single (closure inputFn, mixed prompt, int secure, int lock);
varargs void input_next (closure inputFn, mixed prompt, int secure, int lock);
void input_pop ();
void input_focus ();

/* --- output --- */
void handle_pager (mixed arg);

/* --- shell --- */
object query_shell ();
protected void shell_start ();
protected void shell_stop ();

/*  --- */

#endif /* __USER_H__ */
