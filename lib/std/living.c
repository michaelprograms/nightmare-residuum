inherit STD_OBJECT;
inherit M_CLEAN;
inherit M_CONTAINER;
inherit M_LEVELABLE;
inherit M_CURRENCY;

inherit "/std/living/attribute.c";
inherit "/std/living/biography.c";
inherit "/std/living/body.c";
inherit "/std/living/class.c";
inherit "/std/living/combat.c";
inherit "/std/living/cooldown.c";
inherit "/std/living/protection.c";
inherit "/std/living/stats.c";
inherit "/std/living/status.c";
inherit "/std/living/vitals.c";


int is_living () { return 1; }

void create () {
    combat::create();
    object::create();
}

int handle_remove () {
    protection::handle_remove();
    return clean::handle_remove();
}

void heart_beat () {
    if (!clonep()) {
        return;
    }

    status::heart_beat();
    if (query_hp() > 0) {
        body::heart_beat();
    }
    cooldown::heart_beat();

    handle_combat();

    environment() && environment()->handle_environment_damage(this_object());
}

string query_living_long () {
    string *attributes = ({ });
    string str = "";

    if (query_gender() != "neither" && query_gender() != "none") {
        str = query_gender() + " ";
    }
    str += query_species();
    str = query_cap_name() + " is " + add_article(str);

    foreach (string key,string value in query_attributes()) {
        if (key == "build") {
            attributes += ({ add_article(value) + " build" });
        }
        if (key == "complexion") {
            attributes += ({ add_article(value) + " complexion" });
        }
        if (key == "eye") {
            attributes += ({ value + " eyes" });
        }
        if (key == "hair") {
            attributes += ({ value + " hair" });
        }
        if (key == "height") {
            attributes += ({ value + " units tall" });
        }
    }
    if (sizeof(attributes)) {
        str += " with " + conjunction(attributes);
    }
    str += ".";

    return str;
}

void set_name (string name) {
    object::set_name(name);
    set_long((: query_living_long :));
}

void set_level (int l) {
    levelable::set_level(l);
    vitals::update_vitals(1);
    body::update_limbs(); // limbs require vitals
}

varargs int do_command (string command, int debug) {
    string *split, action, input;
    string cmdPath;

    mixed result, resultGo;

    if (!environment() || !command) {
        return 0;
    }

    if (query_immobile()) {
        message("status", "You are unable to take any actions right now.", this_object());
        return 1;
    }

    if (strsrch(command, "enter") == 0) {
        command = "go " + command;
    } else if (strsrch(command, "out") == 0) {
        command = "go " + command;
    }

    split = explode(command, " ") - ({ "" });
    action = split[0];
    input = sizeof(split) > 1 ? command[(strlen(action)+1)..] : 0;

    if (cmdPath = D_COMMAND->query_command(action)) {
        mixed *parse = parse_command_flags(input);
        catch(call_other(cmdPath + "/" + action, "command", parse[0], parse[1]));
        return 1;
    }

    result = parse_sentence(command, debug);
    if (stringp(result)) {
        write(result + "\n");
        return 1;
    }

    /*
    ** If the result is 0, the parser didn't know the verb so we keep looking.
    ** If a 1 was returned, then nothing more needs to be done.
    ** If the result is -1 or -2, the parser figured something was wrong.
    **
    */
    switch (result) {
    case 0: // Parser found no verb, fall through
        break;
    case 1: // Parser called verb
        return 1;
    case -1:
        write("Nonsense?!\n");
        return 1;
    case -2:
        write("You aren't able to do that.\n");
        return 1;
    default:
        write("Unexpected result.\n");
        return 1;
    }
    if (environment() && environment()->query_exit(command)) {
        resultGo = parse_sentence("go " + command);
        if (resultGo == 1) {
            return 1;
        }
        if (!result) {
            result = resultGo;
        }
    }
    if (stringp(result) && (result[0..12] != "You can't go " && result[0..11] != "There is no ")) {
        write(result + "\n");
        return 1;
    }

    return 0;
}

varargs int handle_go (mixed dest, string verb, string dir, string reverse) {
    string verbs, article, formatReverse;
    object oldEnv, newEnv;
    int move;

    oldEnv = environment();
    newEnv = stringp(dest) ? load_object(dest) : objectp(dest) ? dest : error("Bad argument 1 to living->handle_go");
    verbs = pluralize(verb);
    formatReverse = reverse ? reverse : format_exit_reverse(dir);
    article = member_array(formatReverse, ({ "north", "northeast", "east", "southeast", "south", "southwest", "west", "northwest" })) > -1 ? "the " : "";

    move = handle_move(newEnv);

    message("go", "You " + verb + " %^DIR%^" + dir + "%^DEFAULT%^.", this_object());
    message("go", query_cap_name() + " " + verbs + " %^DIR%^in%^DEFAULT%^ from " + article + "%^DIR%^" + formatReverse + "%^DEFAULT%^.", newEnv->query_living_contents(), this_object());
    message("go", query_cap_name() + " " + verbs + " %^DIR%^" + dir + "%^DEFAULT%^.", oldEnv->query_living_contents(), this_object());

    return move;
}

mixed direct_look_at_liv (mixed args...) {
    return environment() == environment(previous_object());
}
mixed direct_look_liv (mixed args...) {
    return direct_look_at_liv(args);
}
mixed indirect_give_obj_to_liv (object ob, object liv, string obStr, mixed livStr) {
    if (previous_object() == liv && environment(ob) == liv) return "You already have that.";
    return 1;
}
mixed direct_give_wrd_wrd_to_liv (mixed args...) {
    object liv = args[2];
    if (previous_object() == liv) return "You already have that.";
    return 1;
}
mixed indirect_inject_obj_into_liv (mixed args...) {
    return 1;
}