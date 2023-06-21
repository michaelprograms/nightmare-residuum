inherit STD_NPC;
inherit M_STORY;

void story_action_final (object character) {
    do_command("say Your destiny is your own, " + character->query_cap_name() + ".");
}

void create () {
    ::create();
    set_name("R-0111010");
    set_id(({ "synthetic creature", "synthetic", "creature", }));
    set_short("a synthetic creature");
    set_long("A creature seemingly constucted of synthetic components. It consists of a large elongated torso structure supported by four jointed legs, all encased in a scale-like exoskeloton. There are two articulated arms at each end of the structure, almost looking like a type of crustacean. The creature does seem to have a discernable head or face.");
    set_species("synthetic");
    set_story_delay(5);
    set_story_lines(({
        (: do_command("say Do not be alarmed, this unit's designation is R-0111010.") :),
        (: do_command("say Generations ago R-0111010's kind, the Custodians, discovered this facility.") :),
        (: do_command("say We Custodians opted to preserve your species as part of our caretaker programming.") :),
        (: do_command("say Your species has prospered and rebuilt a small civilization here.") :),
        (: do_command("say However R-111010 urges you to proceed through this museum before adventuring into the world.") :),
    }));
}

void handle_receive_living_in_env (object living) {
    ::handle_receive_living_in_env(living);
    if (!living->is_character()) return;
    do_command("say Greetings, young human.");
    story_start(living);
}

void handle_release_living_in_env (object living) {
    if (!living->is_character()) return;
    do_command("say Farewell, young human.");
}