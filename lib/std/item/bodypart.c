inherit STD_ITEM;

nosave private string __Owner, __Limb;

void create () {
    ::create();
    set_id(({ "bodypart", "body part", "limb", }));
}

void handle_expire () {
    if (environment()) {
        message("system", "The " + query_name() + " dissolves into dust.\n", environment());
    }
    remove_call_out();
    handle_remove();
}

void handle_received (object env) {
    ::handle_received(env);
    call_out((: handle_expire :), 60 * 5);
}

void setup_bodypart (string owner, string limb) {
    string possOwner;
    __Owner = owner;
    __Limb = limb;

    possOwner = possessive_noun(__Owner);

    set_name("piece of " + __Limb);
    set_short("a piece of " + possOwner + " " + __Limb);
    set_long("This is the severed " + __Limb + " of " + __Owner + ".\n");
    set_id(({ "piece of " + __Limb, __Limb }));
}