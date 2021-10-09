void command (string input) {
    write(format_header_bar("STATS") + "\n\n");
    foreach(string stat in ({"strength","perception","endurance","charisma","intelligence","agility","luck"})) {
        write(pad_left(stat, 15)+"   "+this_character()->query_stat(stat)+"\n");
    }
    write("\n" + format_footer_bar() + "\n");
}