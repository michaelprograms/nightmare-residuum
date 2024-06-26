#include <human.h>

inherit STD_ITEM;
inherit M_PICKABLE;

void create () {
    ::create();
    set_name("plum tree");
    set_id(({ "tree", "plum tree" }));
    set_adjective(({ "bushy" }));
    set_short("a bushy plum tree");
    set_long("A plum tree.");
    set_no_get(1);

    set_pickable(HUMAN_ITEM + "plum_fruit.c", 5, "A plum ripens on the tree.");
}

void reset () {
    ::reset();
}