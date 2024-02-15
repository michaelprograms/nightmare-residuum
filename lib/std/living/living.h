#ifndef __LIVING_H__
#define __LIVING_H__

/*  ----- living ----- */
int handle_move (mixed dest);
varargs int handle_go (mixed dest, string verb, string dir, string reverse);
varargs int do_command (string str, int debug);
int is_living ();
private void handle_passive_heal ();

/* ----- attribute ----- */
mapping query_attributes ();
string query_attribute (string key);
void set_attribute (string key, string value);

/* ----- biography ----- */
int query_experience ();
int query_total_experience ();
void add_experience (int exp);
void spend_experience (int exp);
int query_victory ();
mixed *query_defeat ();
void handle_victory (object source);
void handle_defeat (object source);

/* ----- body ----- */
string query_gender ();
void set_gender (string str);
string query_species ();
void set_species (string species);
int query_level ();
void set_level (int l);
void update_limbs ();
string *query_limbs ();
mapping query_limb (string limb);
string query_random_limb ();
int query_limb_armor (string limb);
object *query_all_armor ();
object query_worn_shield ();
mixed query_can_wear_armor (object ob);
varargs mixed handle_wear (object armor);
varargs mixed handle_unwear (object armor);
string *query_wieldable_limbs ();
object query_wielded (string limb);
object *query_wielded_weapons ();
string *query_wielded_limbs (object ob);
varargs mixed handle_wield (object weapon, string limb);
varargs mixed handle_unwield (object weapon, string limb);
void heal (int n);
private void handle_passive_heal ();
mapping query_injections ();
int query_injection (string type);
void add_injection (string type, int strength);

/* ----- class ----- */
void set_class (string c);
string query_class ();
void set_sub_class (string c);
string query_sub_class();

/* ----- combat ----- */
mapping *query_combat_table (object target, int hit);
protected void handle_combat ();
private void handle_combat_miss (object target, mixed weapon);
private void handle_combat_block (object target);
private void handle_combat_parry (object target);
private void handle_combat_evade (object target);
private void handle_combat_hit (object target, mixed weapon, int crit);
varargs void check_lifesigns (object source);
varargs int handle_damage (int damage, string limb, object source);
int add_hostile (object ob);
int remove_hostile (object ob);
int query_hostile (object ob);
object *query_hostiles ();
object *query_present_hostiles ();
object query_target_hostile ();

/* ----- protection ----- */
int query_prorection ();
int set_protection (int n);
int clear_protection ();
int add_protection (int n, int time);
int remove_protection (int n);

/* ----- stats ----- */
int query_stat (string stat);
int query_stat_base (string stat);
int query_stat_bonus (string stat);
void set_stat (string stat, int n);
void add_stat_bonus (string stat, int n);

/* ----- status ----- */
void set_disable (int value);
int query_disable ();
void set_busy (int value);
int query_busy ();

/* ----- vitals ----- */
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

#endif /* __LIVING_H__ */