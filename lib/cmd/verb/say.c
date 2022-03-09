inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
}

int can_say () { return 1; }
void do_say () {
    write("Say what?\n");
}

int can_say_str () { return 1; }
void do_say_str (string str) {
    string verb;

    switch (str[<1]) {
        case '?': verb = "ask"; break;
        case '!': verb = "exclaim"; break;
        default: verb = "say";
    }

    message("say", "You "+verb+": "+str+"\n", this_character());
    message("say", this_character()->query_cap_name()+" "+verb+"s: "+str+"\n", environment(this_character()), this_character());
}