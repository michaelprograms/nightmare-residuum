inherit M_SAVE;

private mapping __Emotes = ([ ]);
private string *__Adverbs = ({ });

void create () {
    set_save_path("/save/daemon/soul.o");
    restore_data();

    parse_init();

    foreach (string verb, mapping value in __Emotes) {
        foreach (string rule, mixed ignore in value) {
            catch (parse_add_rule(verb, rule));
        }
    }
}

string *query_emotes () {
    return keys(__Emotes);
}
string *query_adverbs () {
    return __Adverbs;
}

// The fluffos parser accepts six tokens:
//     OBJ - matches a single object
//     OBS - matches one or more objects
//     LIV - matches a single, living object
//     LVS - matches one or more living objects
//     WRD - matches a single word
//     STR - matches one or more words
int add_emote (string verb, mixed rule, string *parts) {
    parse_add_rule(verb, rule);

    if (!__Emotes[verb]) {
        __Emotes[verb] = ([ ]);
    }
    if (sizeof(parts) > 1) {
        __Emotes[verb][rule] = parts;
    } else {
        __Emotes[verb][rule] = parts[0];
    }
    save_data();
    return 1;
}

int remove_emote (string verb, string rule) {
    if (!__Emotes[verb]) {
        return 0;
    }
    if (strlen(rule)) {
        if (!__Emotes[verb][rule]) {
            return 0;
        }
        map_delete(__Emotes[verb], rule);
    } else {
        map_delete(__Emotes, verb);
    }
    parse_remove(verb);
    parse_refresh();
    save_data();
    return 1;
}

// O/o:
// T/t:
// N/n: You/Name
// R/r:
// V/v: pluralize word
// P/p
varargs string compose_message (object forwhom, string msg, object *who, mixed *obs...) {
    mixed *fmt;
    string res;
    int num, subj;
    string str;
    string bit;
    mapping has = ([ ]);

    fmt = reg_assoc(msg, ({ "\\$[OoTtNnRrVvPp][a-z0-9]*" }), ({ 1 }));
    fmt = fmt[0];
    res = fmt[0];
    // write("fmt: "+identify(fmt)+"\n");
    for (int i = 1; i < sizeof(fmt); i += 2) {
        int c = fmt[i][1];
        // write(identify(fmt[i][1..1])+"\n");
        if (fmt[i][2] && fmt[i][2]<'a') {
            if (fmt[i][3] && fmt[i][3] < 'a') {
                subj = fmt[i][2] - '0';
                num = fmt[i][3] - '0';
                str = fmt[i][4..<0];
            } else {
                subj = 0;
                num = fmt[i][2] - '0';
                str = fmt[i][3..<0];
            }
        } else {
            subj = 0;
            num = ((c == 't' || c == 'T') ? 1 : 0); // target defaults to 1, not zero
            str = fmt[i][2..<0];
        }
        switch (c) {
        case 'O':
        case 'o':
            // @TODO
            break;
        case 'T':
        case 't':
            if (str == "") {
                str = "o";
            }
        case 'N':
        case 'n':
            if (str == "") {
                str = "s";
            }
            if (str != "p") {
                if (str != "d") {
                    // Reflexification
                    if (subj < sizeof(who) && who[subj] == who[num] && has[who[subj]]) {
                        if (str == "o") { // Objective: You verb yourself. Name verbs himself.
                            bit = forwhom == who[subj] ? "yourself" : reflexive(who[subj]);
                        } else if (str == "s") { // Subjective: You verb you adjective. Name verbs he adjective.
                            bit = forwhom == who[subj] ? "you" : subjective(who[subj]);
                        }
                        break;
                    }
                    // Other pronouns
                    if (who[num] == forwhom) {
                        bit = "you";
                        has[who[num]] ++;
                        break;
                    }
                    if (has[who[num]]) {
                        bit = str[0] == 'o' ? objective(who[num]) : subjective(who[num]);
                        break;
                    }
                    has[who[num]]++;
                    bit = who[num]->query_cap_name(); // @TODO name vs short
                    break;
                }
            }
            has[who[num]]++;
            bit = who[num]->query_cap_name(); // @TODO name vs short
            break;
        case 'R':
        case 'r':
            // @TODO
            break;
        case 'V':
        case 'v':
            // @TODO is this necessary for emotes? what uses contractions?
            // Contractions
            // if (i + 1 < sizeof(fmt) && fmt[i+1][0..2] == "'t ") {
            //     str += "'t";
            //     fmt[i+1] = fmt[i+1][2..];
            // }
            if (num >= sizeof(who) || who[num] != forwhom) {
                bit = pluralize(str);
            } else {
                bit = str;
            }
            break;
        case 'P':
        case 'p':
            // //Bit of defensive coding here
            // if (num>=sizeof(who)) {
            //     bit = "WHOSE(?)";
            //     break;
            // } else if (forwhom == who[num]) {
            //     bit = "your";
            //     break;
            // }
            // if (has[who[num]]) {
            //     bit = who[num]->query_possessive();
            //     break;
            // }
            // bit = who[num]->query_named_possessive();
            // has[who[num]]++;
            break;
        }
        if (!bit) {
            bit = "";
        }
        if (c < 'a') {
            bit = capitalize(bit);
        }
        res += (bit ? bit : "") + fmt[i+1];
    }
    if (strlen(res) > 0 && res[<1] != '\n') {
        res += "\n";
    }
    return res;
}

varargs mixed *get_soul (string verb, string rule, mixed *args) {
    mapping rules;
    mixed soul;
    mixed *result;
    object *who;
    string tmp;

    if (!(rules = __Emotes[verb]) || !(soul = rules[rule])) {
        return 0;
    }

    if (soul[0] == '=') {
        soul = rules[soul[1..]];
    }
    if (!stringp(soul)) {
    //     if (soul[<1] != '\n') {
    //         soul += "\n";
    //     }
    // } else {
        soul = map_array(soul, (: $1[<1] == '\n' ? $1 : $1 + "\n" :));
    }

    // write("soul == "+identify(soul)+"\n");

    who = ({ this_character() });
    if (strsrch(rule, "LIV") != -1) {
        int i = 0;
        foreach (string token in explode(rule, " ")) {
            if (token == "LIV") {
                who += ({ args[i] });
                args[i..i] = ({});
            } else if (token[0] >= 'A' && token[0] <= 'Z') {
                i ++;
            }
        }
    }
    // if (strsrch(rule, "LVS") != -1) {
    //     int i = 0;
    //     foreach (string token in explode(rule, " ")) {
    //         if (token == "LVS") {
    //             who += ({ args[i] });
    //             args[i..i] = ({});
    //             write("who is now: "+identify(who)+"\n");
    //         } else if (token[0] >= 'A' && token[0] <= 'Z') {
    //             i ++;
    //         }
    //     }
    // }
    result = ({ who, allocate(sizeof(who) + 1) });
    // write("result1: "+identify(result)+"\n");

    for (int i = 0; i < sizeof(who); i ++) {
        if (stringp(soul)) {
            tmp = soul;
        } else if (i && i + 1 < sizeof(soul)) {
            tmp = soul[i + 1];
        } else {
            tmp = soul[0];
        }
        result[1][i] = compose_message(who[i], tmp, who, args...);
    }
    // @TODO is this necessary
    result[1][<1] = compose_message(0, stringp(soul) ? soul : soul[1], who, args...);
    // write("result2: "+identify(result)+"\n");
    return result;
}

void display_soul (object *who, string *msgs, mixed others) {
    mapping done = ([ ]);

    for (int i = 0; i < sizeof(who); i ++) {
        if (done[who[i]]) continue;
        done[who[i]] ++;
        message("soul", who[i], msgs[i]);
    }
    if (arrayp(others)) {
        message("soul", msgs[<1], others, who);
    } else {
        message("soul", msgs[<1], others);
    }
}

int livings_are_remote () {
    return 1;
}

mixed can_verb_wrd (string verb, string wrd) {
    return member_array(wrd, __Adverbs) != -1 || member_array('*', wrd) != -1;
}
mixed can_verb_rule (string verb, string rule) {
    if (!__Emotes[verb]) return;
    return !undefinedp(__Emotes[verb][rule]);
}

mixed direct_verb_rule (string verb, string rule) {
    return !undefinedp(__Emotes[verb][rule]);
}
mixed indirect_verb_rule (string verb, string rule) {
    return !undefinedp(__Emotes[verb][rule]);
}

void do_verb_rule (string verb, string rule, mixed args...) {
    mixed soul;

    // write("do_verb_rule: "+verb+" "+rule+" args: "+identify(args)+"\n");
    soul = get_soul(verb, rule, args);

    if (!soul) {
        return;
    }
    // if (sizeof(soul[0] == 2 && environment(soul[0][1]) == environment(this_character())) {
    //     soul[1][0] = "*" + soul[1][0];
    //     soul[1][1] = "*" + soul[1][1];
    //     inform(soul[0], soul[1], 0);
    // } else {
    // }

    // write("Soul is: "+identify(soul)+"\n");
    display_soul(soul[0], soul[1], environment(this_character())->query_living_contents());
}