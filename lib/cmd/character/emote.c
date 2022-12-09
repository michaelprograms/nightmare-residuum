inherit STD_COMMAND;

void create () {
    set_syntax("emote [input]");
    set_help_text("The emote command is used to send a custom emote message to your character's environment.");
}

void command (string input, mapping flags) {
    object tc = this_character(), *who;
    string myMsg, yourMsg;

    input = strip_colour(input);
    if (!input) {
        message("action", "Emote what?", tc);
        return;
    }

    who = environment(tc)->query_living_contents() - ({ tc });

    myMsg = "You " + input;
    yourMsg = tc->query_cap_name() + input;

    message("soul", myMsg, tc);
    message("soul", yourMsg, who);

    tc->add_say_history(myMsg);
    who->add_say_history(yourMsg);
}