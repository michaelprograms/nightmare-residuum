#define MAX_LEVEL 20

inherit STD_COMMAND;

void create () {
    ::create();
    set_syntax("advance");
    set_help_text("The advance command is used to spend experience points to permanently increase your character's level.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    int cost;

    cost = D_EXPERIENCE->query_level_cost(tc->query_level());
    if (tc->query_experience() < cost) {
        message("action", "You are not yet ready to advance your level.", tc);
        return;
    }

    if (tc->query_level() >= MAX_LEVEL) {
        message("action", "Your level is already at the maximum.", tc);
        return;
    }

    tc->add_experience(-cost);
    tc->set_level(tc->query_level() + 1);
    message("action", "You advance your level to " + tc->query_level() + ".", tc);
    D_LOG->log("character/advance", ctime()+" "+tc->query_key_name()+" to "+tc->query_level());
}