void command (string input) {
    object *hostiles = this_character()->query_hostiles();

    write(format_header_bar("HOSTILES") + "\n");

    if (hostiles && sizeof(hostiles)) {
        foreach (object ob in this_character()->query_hostiles()) {
            write("\n    " + ob->query_name());
        }
        write("\n");
    } else {
        write("\n    No current hostiles\n");
    }

    write("\n" + format_footer_bar() + "\n");
}