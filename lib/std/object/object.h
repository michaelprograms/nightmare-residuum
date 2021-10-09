#ifndef __OBJECT_H__
#define __OBJECT_H__

/* --- object --- */
protected int internal_remove ();
int handle_remove ();
int handle_move (mixed dest);
void create ();
// void heart_beat ();
int id (string id);
int move_or_destruct (object parent);
void reset ();
// void virtual_start ();

/* --- description --- */
nomask void set_long (mixed m);
nomask string query_long ();
nomask void set_short (mixed m);
nomask string query_short ();

/* --- id --- */
string query_name ();
string query_key_name ();
void set_name (string name);
void set_key_name (string name);
string query_plural_name ();
string *query_id ();
string query_key_id ();
void set_id (string *id);
void remove_id (string *id);
string *query_adjective ();
void set_adjective (string *adjs);
void add_adjective (string adj);
void remove_adjective (string adj);
void refresh_id ();
string *parse_command_id_list ();
string *parse_command_adjectiv_id_list ();
string *parse_command_plural_id_list ();
int is_living ();
int inventory_visible ();
int inventory_accessible ();

/* --- property --- */
mapping query_properties ();
mixed query_property (string key);
mixed set_property (string key, mixed value);
mapping set_properties (mapping properties);
mixed add_property (string key, mixed value);
int remove_property (string key);

/*  --- */

#endif /* __OBJECT_H__ */