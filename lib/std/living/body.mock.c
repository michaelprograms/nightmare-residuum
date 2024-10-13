inherit "/std/shadow.c";

inherit "/std/living/stats.c";
inherit "/std/living/vitals.c";

inherit M_CONTAINER;
inherit M_LEVELABLE;

void set_level (int l) {
    levelable::set_level(l);
    vitals::update_vitals(1);
    this_object()->update_limbs(); // limbs require vitals
}