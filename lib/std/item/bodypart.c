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

/**
 * Configure this bodypart off of the living owner passed.
 *
 * @param {STD_LIVING} owner whose bodypart this is
 * @param limb the name of the limb
 */
void setup_bodypart (object owner, string limb) {
    string possOwner;
    __Owner = owner->query_cap_name();
    __Limb = limb;
    possOwner = possessive_noun(__Owner);
    set_name("piece of " + __Limb);
    set_short("piece of " + possOwner + " " + __Limb);
    set_long("This is the severed " + __Limb + " of " + __Owner + ".\n");
    set_id(({ "piece of " + __Limb, __Limb }));
    if (environment(owner)) {
        handle_move(environment(owner));
    }
}