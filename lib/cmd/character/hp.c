inherit STD_COMMAND;

void create () {
    set_syntax("hp");
    set_help_text("The hp command is used to view your character's health and other vitals.");
}

void command (string input, mapping flags) {
    object tc = this_character();
    string hp = tc->query_hp() + "/" + tc->query_max_hp() + " hp";
    string sp = tc->query_sp() + "/" + tc->query_max_sp() + " sp";
    string mp = tc->query_mp() + "/" + tc->query_max_mp() + " mp";
    message("action", "You have " + hp + " " + sp + " " + mp + " and your level is " + tc->query_level() + ".", tc);
}