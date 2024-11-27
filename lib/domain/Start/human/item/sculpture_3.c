#include "human.h"

inherit STD_ITEM;

string sculpture_long () {
    string desc = "";
    object tc = this_character();

    // if (query_account_setting("screenreader") == "on") {
        desc += "It depicts a sculpture.";
    // } else {
        // @TODO
    // }

    D_ACHIEVEMENT->flag(tc, "Art Appreciation", "s3");
    return desc;
}

void create () {
    ::create();
    set_id(({ "sculpture", "david", }));
    set_name("sculpture");
    set_short("a sculpture towering in a southern alcove");
    set_long((: sculpture_long :));
    set_long_footer("It appears to be a recreation of a sculpture from another era of time.");
    set_no_get(1);
}