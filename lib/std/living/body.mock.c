inherit "/std/shadow.c";

inherit "/std/living/stats.c";
inherit "/std/living/vitals.c";
inherit "/std/module/levelable.c";

void set_level (int l) {
    levelable::set_level(l);
    vitals::update_vitals(1);
    this_object()->update_limbs(); // limbs require vitals
}