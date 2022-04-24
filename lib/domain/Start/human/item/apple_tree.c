#include "human.h"

inherit STD_ITEM;
inherit M_PICKABLE;

void create () {
    ::create();
    set_name("apple tree");
    set_id(({"tree", "apple tree" }));
    set_adjective(({ "stout" }));
    set_short("a stout apple tree");
    set_long("A apple tree.");
    set_no_get(1);

    set_pickable(HUMAN_ITEM + "apple_fruit.c", 5, "A apple ripens on the tree.");
}