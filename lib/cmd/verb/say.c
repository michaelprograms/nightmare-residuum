inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
}

int can_say () { return 1; }
void do_say () {
    write("Say what?\n");
}

int can_say_str (mixed args...) { return 1; }
void do_say_str (mixed args...) {
    object po = previous_object();
    string str = args[0];
    string verb;

    switch (str[<1]) {
        case '?': verb = "ask"; break;
        case '!': verb = "exclaim"; break;
        default:
            if (po->query_species() == "synthetic") verb = "synthesize";
            else verb = "say";
    }

    message("say", "You "+verb+": "+str+"\n", previous_object());
    message("say", previous_object()->query_cap_name()+" "+verb+"s: "+str+"\n", environment(previous_object()), previous_object());
}