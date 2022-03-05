void command (string input) {
    object tc = this_character();
    string hp = tc->query_hp() + "/" + tc->query_max_hp() + " hp";
    string sp = tc->query_sp() + "/" + tc->query_max_sp() + " sp";
    string mp = tc->query_mp() + "/" + tc->query_max_mp() + " mp";
    write("You have " + hp + " " + sp + " " + mp + " and your level is " + tc->query_level() + ".\n");
}