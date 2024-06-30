#ifndef __SEFUN_H
#define __SEFUN_H

/* --- sefun --- */
int driver_port ();
string version ();
string mudlib_version ();
string mud_name ();
string call_trace ();

/* --- array --- */
mixed *distinct_array (mixed *arr);
mixed reduce (mixed *arr, function fn, mixed result);

/* --- border --- */
mapping query_border_charset ();
string *format_border (mapping data, mapping b, int width, string ansi);
void border (mapping data);
string *tree (mapping value);

/* --- color --- */
string strip_colour (string str);
string wrap_ansi (string str, int width);
int *query_random_color ();
int color_to_sRGB (float n);
float color_from_sRGB (int n);
float color_lerp (float color1, float color2, float ratio);
string *color_gradient (int *color1, int *color2, int steps);
string apply_gradient (string text, string *gradient);
string format_message_color (string type, string message);

/* ---- convert --- */
int hex_to_int (string b16);
string int_to_binary (int b10);

/* --- combat --- */
void display_combat_message (object source, object target, string limb, mixed weapon, string type, int damage, int crit, int isAbility);
void display_heal_message (object source, object target, string limb, int damage);
void initiate_combat (object source, object target);

/* --- format --- */
varargs string format_page (string *items, mixed columns, int pad, int center, string ansi);
string format_syntax (string text);
string format_exit_brief (string dir);
string format_exit_verbose (string dir);
string format_exit_reverse (string dir);
string format_stat_brief (string stat);
string format_stat_verbose (string stat);
string format_integer (int num);

/* --- grammar --- */
string remove_article (string str);
varargs string add_article (string str, int definite);
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

/* --- json --- */
varargs string json_encode (mixed value, mixed *refs);

/* --- noise --- */
int *noise_generate_permutation (string seed);
float noise_perlin_2d (float x, float y, int *p, int octaves, float scale);
float noise_perlin_3d (float x, float y, float z, int *p, int octaves, float scale);
mapping noise_generate_permutation_simplex (string seed);
float noise_simplex_4d (float x, float y, float z, float w, mapping p, int octaves, float scale);
float gradient_2d (int x1, int y1, int x2, int y2, float x, float y);

/* --- override --- */
nomask varargs int input_to ();
nomask object this_player (int flag);
object *users ();
nomask void write (string msg);
varargs void message (mixed type, string message, mixed target, mixed exclude);

/* --- path --- */
string user_path (string name);
string *split_path (string path);
string sanitize_path (string path);
varargs string absolute_path (string relative_path, mixed relative_to);
int mkdirs (string path);
string *wild_card (string path);

/* --- roll --- */
mixed *roll_die (int times, int sides);

/* --- string --- */
string identify (mixed a);
varargs string wrap (string str, int width, int indent, int rawANSI);
int string_compare_same_until (string a, string b);
string sanitize_name (string name);
mixed *parse_command_flags (string rawInput);

/* --- time --- */
varargs string time_ago (int timestamp, int granularity);

/* --- user --- */
object this_user ();
object find_user (string name);
object this_character ();
object find_character (string name);
object *characters ();
string query_account_setting (string setting);

#endif /* SEFUN_H */
