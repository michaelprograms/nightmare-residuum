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

/* --- format --- */
varargs string format_header_bar (string title, string optional);
string format_divider_bar ();
string format_footer_bar ();

/* --- grammar --- */
string remove_article (string str);
string cardinal (int n);
string pluralize (mixed single);
string consolidate (int n, string str);
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

/* --- string --- */
string strip_colour (string str);
varargs string center (string str, int width);
varargs string pad_left (string text, int n, string pad);
varargs string pad_right (string text, int n, string pad);
varargs string identify (mixed a);
varargs string wrap (string str, int width, int indent);
int string_compare_same_until (string a, string b);
string sanitize_name (string name);

/* --- tell --- */
varargs void tell (mixed target, string msg);

/* --- time --- */
varargs string time_ago (int timestamp, int granularity);

/* --- user --- */
object this_account ();
object this_character ();
object find_character (string name);

#endif /* SEFUN_H */
