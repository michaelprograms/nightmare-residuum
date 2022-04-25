#include "human.h"

inherit STD_ITEM;
inherit M_PICKABLE;

void create () {
    ::create();
    set_name("peach tree");
    set_id(({"tree", "peach tree" }));
    set_adjective(({ "short" }));
    set_short("a short peach tree");
    set_long("A peach tree.");
    set_no_get(1);

    set_pickable(HUMAN_ITEM + "peach_fruit.c", 5, "A peach ripens on the tree.");
}