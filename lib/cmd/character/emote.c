inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("emote [input]");
    set_help_text("The emote command is used to send a custom emote message to your character's environment.");
}

void command (string input, mapping flags) {
    string myMsg, yourMsg;
    object tc = this_character();
    /** @type {STD_CHARACTER*} */
    object *who;
    /** @type {STD_ROOM} */
    object env;

    input = strip_colour(input);
    if (!input) {
        message("action", "Emote what?", tc);
        return;
    } else if (input[<1..] != ".") {
        input += ".";
    }

    env = environment(tc);
    who = env->query_living_contents() - ({ tc });

    myMsg = "You " + input;
    yourMsg = tc->query_cap_name() + input;

    message("soul", myMsg, tc);
    message("soul", yourMsg, who);

    tc->add_say_history(myMsg);
    who->add_say_history(yourMsg);
}