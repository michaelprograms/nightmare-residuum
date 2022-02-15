void command (string input) {
    mixed inventory = unique_array(all_inventory(this_character()), (: $1->query_short() :));
    mixed *list;
    string *shorts;

    write(format_header_bar("INVENTORY") + "\n\n");

    list = unique_array(all_inventory(this_character()), (: $1->query_short() :));
    if (sizeof(list)) {
        shorts = sort_array(map_array(list, (: capitalize(consolidate(sizeof($1), $1[0]->query_short())) :)), 1);
        shorts = map_array(shorts, (: $1 :));
        message("action", implode(shorts, "\n") + "\n\n", this_object());
        foreach (string short in shorts) {
            message("action", "  " + short + "%^RESET%^\n", this_character());
        }
    }

    write("\n" + format_footer_bar() + "\n");
}