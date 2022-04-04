#include "living/living.h"

inherit STD_OBJECT;
inherit M_CLEAN;
inherit M_CONTAINER;

inherit "/std/living/biography.c";
inherit "/std/living/body.c";
inherit "/std/living/combat.c";
inherit "/std/living/skills.c";
inherit "/std/living/stats.c";
inherit "/std/living/vitals.c";

inherit M_CURRENCY;

nosave private int __NextHeal;

int is_living () { return 1; }

void create () {
    combat::create();
    object::create();
}

void heart_beat () {
    if (!clonep()) return;

    handle_passive_heal();
    handle_combat();
}

private void handle_passive_heal () {
    if (!__NextHeal) {
        __NextHeal = time() + 10;
        return;
    } else if (__NextHeal <= time()) {
        int amt = to_int(ceil((query_level() / 5.0) + (query_stat("endurance") / 10.0)));
        add_hp(amt);
        add_sp(amt);
        add_mp(amt);
        __NextHeal = time() + 10;
    }
}

varargs int do_command (string str, int debug) {
    // string action;
    // string verb, input;
    mixed result, resultGo;

    // sscanf(str, "%s %s", verb, input);
    // action = D_COMMAND->query_command(verb);
    // if(stringp(action)) {
    //     call_other(action, "command", input); // @TODO is this right here?
    // } else
    if (!environment()) {
        return 0;
    }

    if (strsrch(str, "enter") == 0) {
        str = "go " + str;
    } else if (strsrch(str, "out") == 0) {
        str = "go " + str;
    }

    result = parse_sentence(str, debug);
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
    if (environment() && environment()->query_exit(str)) {
        resultGo = parse_sentence("go " + str);
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

int handle_go (mixed dest, string verb, string dir) {
    string verbs, article;
    object oldEnv, newEnv;
    int move;

    oldEnv = environment();
    newEnv = stringp(dest) ? load_object(dest) : objectp(dest) ? dest : error("Bad argument 1 to living->handle_go");
    verbs = pluralize(verb);
    article = (!regexp(dir, "^(enter|out|down|up)") ? "the " : "");

    move = handle_move(newEnv);

    message("go", "You " + verb + " %^DIR%^" + dir + "%^DEFAULT%^.\n", this_object());
    message("go", query_cap_name() + " " + verbs + " %^DIR%^in%^DEFAULT%^ from " + article + "%^DIR%^" + format_exit_reverse(dir) + "%^DEFAULT%^.\n", newEnv->query_living_contents(), this_object());
    message("go", query_cap_name() + " " + verbs + " %^DIR%^" + dir + "%^DEFAULT%^.\n", oldEnv->query_living_contents(), this_object());

    return move;
}

int handle_receive (object ob) {
    if (ob) {
        if (ob->is_currency()) {
            foreach (string currency in ob->query_currencies()) {
                int n = ob->query_currency(currency);
                add_currency(currency, n);
                ob->add_currency(currency, -n);
            }
            // call out to delay fn til after move
            call_out_walltime((: $(ob)->check_empty() :), 0);
        }
    }
    return ::handle_receive(ob);
}


mixed direct_look_at_liv () {
    return environment() == environment(previous_object());
}
mixed direct_look_liv () {
    return direct_look_at_liv();
}
mixed indirect_give_obj_to_liv (object ob, object liv, string obStr, mixed livStr) {
    if (previous_object() == liv && environment(ob) == liv) return "You already have that.";
    return 1;
}
mixed direct_give_wrd_wrd_to_liv (mixed args...) {
    string word1 = args[0];
    string word2 = args[1];
    object liv = args[2];
    if (previous_object() == liv) return "You already have that.";
    return 1;
}