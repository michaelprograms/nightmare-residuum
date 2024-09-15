inherit STD_VERB;

void create () {
    verb::create();
    add_rules(({ "", "STR" }));
    set_syntax("say [message]");
    set_help_text("Your character will attempt to speak the provided message.");
}

int can_say () { return 1; }
void do_say () {
    write("Say what?\n");
}

string format_npc_message(object npc, string msg) {
    int pos, l;
    foreach (string say in npc->query_say_response_matches()) {
        pos = strsrch(msg, say);
        if (pos > 0) {
            l = sizeof(say);
            msg = msg[0..pos-1] + "%^UNDERLINE%^" + msg[pos..pos+l-1] + "%^RESET%^" + msg[pos+l..];
        }
    }
    return msg;
}

int can_say_str (mixed args...) { return 1; }
void do_say_str (mixed args...) {
    object po = previous_object(), *who;
    string msg, verb, myMsg, yourMsg;

    if (!sizeof(args)) return;
    msg = args[0];

    switch (msg[<1]) {
        case '?': verb = "ask"; break;
        case '!': verb = "exclaim"; break;
        default:
            if (po->query_species() == "custodian") verb = "synthesize";
            else verb = "say";
    }
    who = environment(po)->query_living_contents() - ({ po });

    if (po->is_npc()) {
        msg = format_npc_message(po, msg);
    }

    myMsg = "You " + verb + ": " + msg;
    yourMsg = po->query_cap_name() + " " + verb + "s: " + msg;

    message("say", myMsg, po);
    message("say", yourMsg, who);

    po->add_say_history(myMsg);
    who->add_say_history(yourMsg);
}