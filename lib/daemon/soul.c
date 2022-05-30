inherit M_SAVE;

private mapping __Emotes = ([ ]);
nosave private mapping __EmotesDefault = ([ ]);

private void generate_defaults () {
    string *list = ({ "smile", "frown", "nod", "cheer", "scream", "fingerguns", "laugh", "grin", "scowl", "chuckle", "point", });

    foreach (string emote in list) {
        __EmotesDefault[emote] = ([
            "": "$N $v" + emote + ".",
            "OBJ": "$N $v" + emote + " at $o.",
            "LIV": "$N $v" + emote + " at $t.",
            "LVS": "$N $v" + emote + " at $O.",
        ]);
    }
}

void create () {
    parse_init();
    set_save_path("/save/daemon/soul.o");
    restore_data();

    generate_defaults();

    foreach (string verb, mapping value in __EmotesDefault + __Emotes) {
        foreach (string rule, mixed ignore in value) {
            catch (parse_add_rule(verb, rule));
        }
    }
}

mapping query_emote (string verb) {
    return __Emotes[verb] || __EmotesDefault[verb];
}
string *query_emotes () {
    return keys(__Emotes) + keys(__EmotesDefault);
}

// The fluffos rule parser accepts six tokens:
//     OBJ - matches a single object
//     OBS - matches one or more objects
//     LIV - matches a single, living object
//     LVS - matches one or more living objects
//     WRD - matches a single word
//     STR - matches one or more words
int add_emote (string verb, mixed rule, string format) {
    parse_add_rule(verb, rule);

    if (!__Emotes[verb]) {
        __Emotes[verb] = ([ ]);
    }
    __Emotes[verb][rule] = format;
    save_data();
    return !!query_emote(verb);
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

// O/o: Object list
// T/t: Objective Name?
// N/n: You/Name
// R/r: Reflexive   your/him/her/them/it + self
// V/v: pluralize word
// P/p: Possessive
varargs string parse_emote (object forwhom, string msg, object *who, mixed *args) {
    mixed *fmt, obs;
    string emote = "", tmp, names;
    mapping has = ([ ]);

    // find msg's replacement flags
    fmt = reg_assoc(msg, ({ "\\$[OoTtNnRrVvPp][a-z0-9]*" }), ({ 1 }));
    fmt = fmt[0];

    // loop thru all replacement flags
    for (int i = 1; i < sizeof(fmt); i += 2) {
        int c = fmt[i][1]; // grab first character of this flag
        int num, subj;

        if (fmt[i][2] && fmt[i][2] < 'a') {
            if (fmt[i][3] && fmt[i][3] < 'a') {
                subj = fmt[i][2] - '0';
                num = fmt[i][3] - '0';
                tmp = fmt[i][4..<0];
            } else {
                subj = 0;
                num = fmt[i][2] - '0';
                tmp = fmt[i][3..<0];
            }
        } else {
            subj = 0;
            num = (c == 't' || c == 'T') ? 1 : 0; // target defaults to 1, not zero
            tmp = fmt[i][2..<0];
        }

        switch (c) {
        case 'O':
        case 'o':
            obs = args[num];
            if (objectp(obs)) obs = ({ obs });
            names = conjunction(map(obs, (: $1 ? ($1->is_living() ? ($1 == $(forwhom) ? "you" : $1->query_cap_name()) : $1->query_name()) : 0 :)));
            break;
        case 'T':
        case 't':
            if (tmp == "") tmp = "o"; // default to objective
        case 'N':
        case 'n':
            if (tmp == "") tmp = "s"; // default is subjective
            if (tmp != "p") {
                if (tmp != "d") {
                    // Reflexification
                    if (subj < sizeof(who) && who[subj] == who[num] && has[who[subj]]) {
                        if (tmp == "o") { // Objective: You verb yourself. Name verbs himself.
                            names = forwhom == who[subj] ? "yourself" : reflexive(who[subj]);
                        } else if (tmp == "s") { // Subjective: You verb you adjective. Name verbs he adjective.
                            names = forwhom == who[subj] ? "you" : subjective(who[subj]);
                        }
                        break;
                    }
                    // Other pronouns
                    if (who[num] == forwhom) {
                        names = "you";
                        has[who[num]] ++;
                        break;
                    }
                    if (has[who[num]]) {
                        names = tmp[0] == 'o' ? objective(who[num]) : subjective(who[num]);
                        break;
                    }
                    has[who[num]]++;
                    names = who[num]->query_cap_name();
                    break;
                }
            }
            has[who[num]]++;
            names = who[num]->query_cap_name();
            break;
        case 'R':
        case 'r':
            // @TODO
            break;
        case 'V':
        case 'v':
            if (num >= sizeof(who) || who[num] != forwhom) {
                names = pluralize(tmp);
            } else {
                names = tmp;
            }
            break;
        case 'P':
        case 'p':
            // @ TODO
            break;
        }
        if (!names) names = "";
        if (c < 'a') names = capitalize(names);
        emote += (names ? names : "") + fmt[i+1];
    }
    if (strlen(emote) > 0 && emote[<1] != '\n') {
        emote += "\n";
    }
    return emote;
}

varargs private mapping prepare_emote (string verb, string rule, mixed *args) {
    mapping result, rules;
    mixed ruleFormat;
    object *who;

    if (!(rules = query_emote(verb)) || !(ruleFormat = rules[rule])) {
        return 0;
    }

    who = ({ this_character() });
    if (strsrch(rule, "LIV") != -1) {
        foreach (string token in explode(rule, " ")) {
            if (token == "LIV") {
                who += ({ args[0] });
            }
        }
    } else if (strsrch(rule, "LVS") != -1) {
        foreach (string token in explode(rule, " ")) {
            if (token == "LVS") {
                who += ({ args[0]... });
            }
        }
    }
    result = ([
        "who": who,
        "msgs": allocate(sizeof(who) + 1),
        "env": environment(this_character())->query_living_contents() - ({ this_character() }),
    ]);

    for (int i = 0; i < sizeof(who); i ++) {
        result["msgs"][i] = parse_emote(who[i], ruleFormat, who, args);
    }

    if (!result["msgs"][<1]) {
        result["msgs"][<1] = parse_emote(0, stringp(ruleFormat) ? ruleFormat : ruleFormat[1], who, args);
    }

    return result;
}

private void display_emote (object *who, string *msgs, mixed others) {
    mapping done = ([ ]);

    for (int i = 0; i < sizeof(who); i ++) {
        if (done[who[i]]) continue;
        done[who[i]] ++;
        message("soul", msgs[i], who[i]);
    }
    if (arrayp(others)) {
        message("soul", msgs[<1], others, who);
    } else {
        message("soul", msgs[<1], others);
    }
}

/* ----- parser applies ----- */

mixed can_verb_rule (mixed args...) {
    string verb, rule;
    mapping emote;

    if (sizeof(args) < 2) return;
    verb = args[0];
    rule = args[1];

    if (!(emote = query_emote(verb))) return 0;
    return !undefinedp(emote[rule]);
}

mixed direct_verb_rule (mixed args...) {
    string verb, rule;

    if (sizeof(args) < 2) return 0;
    verb = args[0];
    rule = args[1];

    if (rule == "LVS" && sizeof(args) > 2) {
        object po = previous_object();
        object who = args[2];
        if (po == who || environment(po) != environment(who) || !who->is_living()) return 0;
    } else if (rule == "OBJ" && sizeof(args) > 2) {
        object po = previous_object();
        object who = args[2];
        if (po == who || environment(po) != environment(who) || who->is_living()) return 0;
    }

    return !undefinedp(query_emote(verb)[rule]);
}

void do_verb_rule (mixed args...) {
    string verb, rule;
    mapping emote;

    if (sizeof(args) < 2) return;
    verb = args[0];
    rule = args[1];

    emote = prepare_emote(verb, rule, args[2..]);
    if (!emote) return;
    display_emote(emote["who"], emote["msgs"], emote["env"]);
}