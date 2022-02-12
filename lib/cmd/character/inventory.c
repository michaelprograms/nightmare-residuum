void command (string input) {

    write(format_header_bar("INVENTORY") + "\n\n");
    foreach (object ob in all_inventory(this_character())) {
        message("action", "  " + ob->query_short() + "%^RESET%^\n", this_character());
    }
    write("\n" + format_footer_bar() + "\n");
}