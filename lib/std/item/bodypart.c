inherit STD_ITEM;
inherit M_DUSTABLE;

nosave private string __Owner, __Limb;

void create () {
    ::create();
    set_id(({ "bodypart", "body part", "limb", }));
}

void handle_received (object env) {
    dustable::handle_received(env);
}

void setup_bodypart (string owner, string limb) {
    string possOwner;
    __Owner = owner;
    __Limb = limb;

    possOwner = possessive_noun(__Owner);

    set_name("piece of " + __Limb);
    set_short("piece of " + possOwner + " " + __Limb);
    set_long("This is the severed " + __Limb + " of " + __Owner + ".\n");
    set_id(({ "piece of " + __Limb, __Limb }));
}