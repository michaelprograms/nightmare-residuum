inherit "/std/item/tree_pickable.c";

void create () {
    ::create();
    set_name("plum tree");
    set_id(({"tree", "plum tree" }));
    set_adjective(({ "bushy" }));
    set_short("a bushy plum tree");
    set_long("A plum tree.");
    // @TODO set_no_get("You cannot get that.");
    set_max_picks(5);
    set_pickable("/domain/Start/human/item/plum_fruit.c");
}