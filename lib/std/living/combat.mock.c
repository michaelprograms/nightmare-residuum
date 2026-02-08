inherit STD_OBJECT;
inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_LEVELABLE;
inherit M_CURRENCY;

inherit "/std/living/attribute.c";
inherit "/std/living/biography.c";
inherit "/std/living/body.c";
inherit "/std/living/class.c";
inherit "/std/living/command.c";
inherit "/std/living/cooldown.c";
inherit "/std/living/protection.c";
inherit "/std/living/stats.c";
inherit "/std/living/status.c";
inherit "/std/living/vitals.c";

inherit "/std/shadow.c";

void heart_beat () {
    status::heart_beat();
    cooldown::heart_beat();
}