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

    tc->set_achievement_flag("Art Appreciation", "s2", (HUMAN_BASE + "achievements.c")->query_achievement_flags("Art Appreciation"));
    return desc;
}

// TODO: need transparent background of this
// Umberto Boccioni, Unique Forms of Continuity in Space, 1913
// From its radical beginnings to its final fascist incarnation, Italian Futurism shocked the world, but no single work exemplified the sheer delirium of the movement than this sculpture by one of its leading lights: Umberto Boccioni. Starting out as a painter, Boccioni turned to working in three dimensions after a 1913 trip to Paris in which he toured the studios of several avant-garde sculptors of the period, such as Constantin Brancusi, Raymond Duchamp-Villon and Alexander Archipenko. Boccioni synthesized their ideas into this dynamic masterpiece, which depicts a striding figure set in a “synthetic continuity” of motion as Boccioni described it. The piece was originally created in plaster and wasn’t cast in its familiar bronze version until 1931, well after the artist’s death in 1916 as a member of an Italian artillery regiment during World War I.

void create () {
    ::create();
    set_id(({ "sculpture", "david", }));
    set_name("sculpture");
    set_short("a sculpture towering in a eastern alcove");
    set_long((: sculpture_long :));
    set_long_footer("It appears to be a recreation of a sculpture from another era of time.");
    set_no_get(1);
}