inherit STD_NPC;

void create () {
    ::create();
    set_name("robot creature");
    set_id(({"creature", "robot", "robot creature"}));
    set_short("a robot creature");
    set_long("A robot creature with a large torso component supported by four jointed legs. There are two articulated arms at each end of the torso. The robot does seem to have a head or face.");
    set_species("synthetic");
}

void greet_character (object living, string message) {
    do_command("say " + message);
}

void handle_receive_living_in_env (object living) {
    if (!living->is_character()) return;
    greet_character(living, "Hello.");
}

void handle_release_living_in_env (object living) {
    if (!living->is_character()) return;
    greet_character(living, "Goodbye.");
}