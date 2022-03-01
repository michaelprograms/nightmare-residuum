void command (string input) {
    object tc = this_character();
    this_character()->set_hp(this_character()->query_max_hp());
    this_character()->set_sp(this_character()->query_max_sp());
    this_character()->set_mp(this_character()->query_max_mp());
    write("Health vitals returned to full.\n");

}