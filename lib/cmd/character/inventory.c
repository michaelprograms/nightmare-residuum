void command (string input, mapping flags) {
    mixed *list;
    string *shorts, *coins = ({});

    message("action", format_header_bar("INVENTORY") + "\n\n", this_character());

    list = unique_array(all_inventory(this_character()), (: $1->query_short() :));
    if (sizeof(list)) {
        shorts = sort_array(map_array(list, (: capitalize(consolidate(sizeof($1), $1[0]->query_short())) :)), 1);
        shorts = map_array(shorts, (: $1 :));
        message("action", implode(shorts, "\n") + "\n\n", this_object());
        foreach (string short in shorts) {
            message("action", "  " + short + "%^RESET%^\n", this_character());
        }
    }

    foreach (string currency in this_character()->query_currencies()) {
        int n;
        if (n = this_character()->query_currency(currency)) {
            coins += ({ n + " " + currency });
        }
    }
    if (sizeof(coins)) {
        message("action", "  Currency: " + conjunction(coins) + "\n", this_character());
    }

    message("action", "\n" + format_footer_bar() + "\n", this_character());
}