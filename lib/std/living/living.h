#ifndef __LIVING_H__
#define __LIVING_H__

/*  --- living --- */
int handle_move (mixed dest);
int handle_go (mixed dest, string method, string dir);
varargs int do_command (string str, int debug);
int is_living ();
private void handle_passive_heal ();

/* --- body --- */
string query_gender ();
void set_gender (string str);
string query_species ();
void set_species ();
int query_level ();
void set_level (int l);
int query_experience ();
int query_total_experience ();
void add_experience (int exp);
void spend_experience (int exp);
void add_victory ();
void add_defeat ();
int query_victory ();
int query_defeat ();
void handle_defeat (int keep);
private void update_limbs ();
string *query_limbs ();
mapping query_limb (string limb);
string *query_wieldable_limbs ();
object query_wielded (string limb);
object *query_wielded_weapons ();
varargs int handle_wield (object weapon, string limb);
varargs int handle_unwield (object weapon, string limb);

/* --- combat --- */
protected void handle_combat ();
private void handle_combat_hit (object target, mixed weapon);
varargs void check_lifesigns (object source);
int handle_damage (int damage);
int add_hostile (object ob);
int remove_hostile (object ob);
int query_hostile (object ob);
object *query_hostiles ();
object *query_present_hostiles ();
object query_target_hostile ();

/* --- location --- */
string query_last_location ();
void set_last_location (string location);

/* --- skills --- */
private mapping query_default_skills ();
private int query_skill_next_points (string skill);
string *query_all_skills ();
string query_skill_progress (string skill);
int query_skill (string skill);
void set_skill (string skill, int level);
varargs void train_skill (string skill, float multiplier);

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