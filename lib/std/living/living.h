#ifndef __LIVING_H__
#define __LIVING_H__

/*  --- living --- */
string query_last_environment ();
string query_last_location ();
void set_last_location (string location);
int handle_move (mixed dest);
int handle_go (mixed dest, string method, string dir);
varargs int do_command (string str, int debug);
int is_living ();
private void handle_passive_heal ();

/* --- body --- */
string query_gender ();
void set_gender (string str);
int query_level ();
void set_level (int l);

/* --- combat --- */
int add_hostile (object ob);
int remove_hostile (object ob);
object *query_hostiles ();

/* --- stats --- */
int query_stat (string stat);
void set_stat (string stat, int n);

/* --- vitals --- */
int query_hp ();
int query_sp ();
int query_mp ();
int query_max_hp ();
int query_max_sp ();
int query_max_mp ();
void add_hp (int n);
void add_sp (int n);
void add_mp (int n);
void set_hp (int n);
void set_sp (int n);
void set_mp (int n);
void update_vitals (int heal);

/*  --- */

#endif /* __LIVING_H__ */