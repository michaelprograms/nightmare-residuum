#ifndef __SEFUN_H
#define __SEFUN_H

/* --- sefun --- */
int driver_port ();
string driver_version ();
string mudlib_version ();
string mud_name ();

/* --- access --- */
mixed unguarded (function f);

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
string ordinal (int n);
string pluralize (mixed single);
string consolidate (int n, string str);
string conjunction (string *list);
string possessive_noun (mixed value);
string subjective (mixed value);
string objective (mixed value);
string possessive (mixed value);
string reflexive (mixed value);

/* --- override --- */
nomask varargs int input_to ();
nomask object this_player (int flag);
object *users ();
nomask void write (string msg);

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
