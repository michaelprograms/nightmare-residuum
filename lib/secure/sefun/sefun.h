#ifndef __SEFUN_H
#define __SEFUN_H

/* --- sefun --- */
int driver_port ();
string driver_version ();
string mudlib_version ();
string mud_name ();
varargs void message(string type, string message, mixed target, mixed exclude);
string *keys (mapping m);
object this_user ();

/* --- access --- */
mixed unguarded (closure f);

/* --- array --- */
mixed *distinct_array (mixed *arr);

/* --- color --- */
int hex_to_int (string base16);

/* --- combat --- */
void display_combat_message (object source, object target, string limb, mixed weapon, string type, int damage);

/* --- format --- */
string format_exit_brief (string dir);
string format_exit_verbose (string dir);
string format_stat_brief (string stat);
string format_stat_verbose (string stat);
string format_exit_reverse (string dir);
string format_integer (int num);
varargs string format_page (string *items, int columns, int pad);
string *format_border (mapping data);

/* --- grammar --- */
string remove_article (string str);
string cardinal (int n);
string pluralize (mixed single);
string consolidate (int n, string str);
string conjunction (string *list);
varargs string possessive_noun (mixed value);
varargs string subjective (mixed value);
varargs string objective (mixed value);
varargs string possessive (mixed value);
varargs string reflexive (mixed value);

/* --- override --- */
varargs int input_to ();
// varargs nomask object this_player (int flag);
object *users ();
void write (string msg);

/* --- path --- */
string user_path (string name);
string *split_path (string path);
string base_path (string path);
string sanitize_path (string path);
varargs string absolute_path (string relative_path, mixed relative_to);
int assure_dir (string path);
string *wild_card (string path, string relative_to);

/* --- string --- */
string strip_colour (string str);
string identify (mixed a);
varargs string wrap (string str, int width, int indent);
int string_compare_same_until (string a, string b);
string sanitize_name (string name);

/* --- time --- */
varargs string time_ago (int timestamp, int granularity);

/* --- user --- */
object this_account ();
object this_character ();
object find_character (string name);
object *characters ();

#endif /* SEFUN_H */
