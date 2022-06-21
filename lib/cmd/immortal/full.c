void command (string input, mapping flags) {
    object tc = this_character();
    tc->set_hp(tc->query_max_hp());
    tc->set_sp(tc->query_max_sp());
    tc->set_mp(tc->query_max_mp());
    write("Health vitals returned to full.\n");
}