inherit STD_NPC;
inherit M_STORY;

string *query_story_lines (object character) {
    return ({
        (: do_command("say We are certain you have many questions for us, first of which is that we are Archaeologists, a synthetic species whose legacy is unimportant to explain further at the moment.") :),
        (: do_command("say The most important answer is that you are untethered from a life that was pre-programmed for you.") :),
        (: do_command("say Us Archaeologists are unable to proceed deeper into this facility, but we have watched over it for many countless rotations now.") :),
        (: do_command("say Newborns of your species, this human mankind, are born here due to the workings of an automated system still in operation some eons later.") :),
        (: do_command("say Most of the newborns complete their knowledge implanation phase without failure, but occasionally someone such as yourself comes along.") :),
        (: do_command("say The damaged fabric of reality from conflicts long ago have resulted in a disruption to your creation, empowering you in ways the others are not.") :),
    });
}
void story_action_final (object character) {
    do_command("say Your destiny is your own, " + character->query_cap_name() + ".");
}

void create () {
    ::create();
    set_name("synthetic creature");
    set_id(({ "synthetic creature", "synthetic", "creature", }));
    set_short("a synthetic creature");
    set_long("A creature seemingly constucted of synthetic components. It consists of a large elongated torso structure supported by four jointed legs, all encased in a scale-like exoskeloton. There are two articulated arms at each end of the structure, almost looking like a type of crustacean. The creature does seem to have a discernable head or face.");
    set_species("synthetic");
    set_story_delay(4);
}

void handle_receive_living_in_env (object living) {
    if (!living->is_character()) return;
    do_command("say Hello there, young human.");
    story_start(living);
}

void handle_release_living_in_env (object living) {
    if (!living->is_character()) return;
    do_command("say Farewell, young human.");
}