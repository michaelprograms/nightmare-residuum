inherit STD_ITEM;

void create () {
    ::create();
    set_name("body");
    set_id(({"body", "light"}));
    set_short("a cone of %^CYAN%^light%^DEFAULT%^ over a body");
    set_long("The strange body of an unknown species that looks as if it were asleep next to an open conduit panel. It has four legs and two arms attached to a central torso, with a large nub near the top of the body. There looks to be burn marks over a significant portion of skin. The conduit panel has a stone outer side but is easily discernable while flipped open. There are several devices and tools next to the conduit panel. A small device above projects a blue cone of %^CYAN%^BOLD%^light%^RESET%^ over both the body and conduit panel.");
    // @TODO set_no_get("You cannot get that.");
}