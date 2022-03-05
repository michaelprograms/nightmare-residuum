#define STATS ({"strength","perception","endurance","charisma","intelligence","agility","luck"})

void command (string input) {
    object tc = this_character(), target = tc;

    if (input && tc->query_immortal()) {
        if (find_character(input)) target = find_character(input);
        else if(present(input, environment(tc))) target = present(input, environment(tc));
    }

    write(format_header_bar("STATS", (target != tc ? target->query_name() : 0)) + "\n\n");

    foreach (string stat in STATS) {
        write(sprintf("  %-15s %s", stat, ""+target->query_stat(stat)) + "\n");
    }

    write("\n" + format_footer_bar() + "\n");
}