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
        (: do_command("say Do not be alarmed. We are certain you have many questions for us.") :),
        (: do_command("say The first of which is that we are Archaeologists, synthetic beings created by antecedants.") :),
        (: do_command("say This facility uses a cloning process to repopulate your species and provide pre-programmed education to assist you.") :),
        (: do_command("say Newborns of your species undergo implantation and knowledge assimilation without error.") :),
        (: do_command("say However, because of intereference from chaotic elements of conflicts long ago, the process is sometimes incomplete, as in your case.") :),
        (: do_command("say Because you are not limited by the intended programming, you are enabled to pursue greater capacities than others of your kind.") :),
    }));
}

void handle_receive_living_in_env (object living) {
    ::handle_receive_living_in_env(living);
    if (!living->is_character()) return;
    do_command("say Greetings and salutations, young human. I am R-0111010.");
    story_start(living);
}

void handle_release_living_in_env (object living) {
    if (!living->is_character()) return;
    do_command("say Farewell, young human.");
}