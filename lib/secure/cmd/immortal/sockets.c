void command (string input) {
    string ret = @END
    Fd    State      Mode       Local Address          Remote Address
    --  ---------  --------  ---------------------  ---------------------
END;

    write(format_header_bar("SOCKETS", mud_name()) + "\n\n");
    write(ret);
    foreach (mixed *item in socket_status()) {
        write(sprintf("    %2d  %|9s  %|8s  %-21s  %-21s\n", item[0], item[1], item[2], item[3], item[4]));
    }

    write("\n");

    foreach(string key, int value in filter_array(network_stats(), (:strsrch($1, "socket") > -1:))) {
        write("    " + sprintf("%20s", key) + " : " + value + "\n");
    }

    write("\n" + format_footer_bar() + "\n");
}