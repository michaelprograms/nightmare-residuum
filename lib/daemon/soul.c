inherit M_SAVE;

private mapping __Emotes = ([ ]);
nosave private mapping __EmotesDefault = ([ ]);

private void generate_defaults () {
    string *list = ({ "smile", "frown", "nod", "cheer", "scream", "fingerguns", "laugh", "grin", "scowl", "chuckle", });

    foreach (string emote in list) {
        __EmotesDefault[emote] = ([
            "": "$N $v" + emote + ".",
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
varargs string compose_message (object forwhom, string msg, object *who, mixed *args...) {
    mixed *fmt;
    string res;
    int num, subj;
    string str;
    string bit;
    mapping has = ([ ]);
    object *obs;

    fmt = reg_assoc(msg, ({ "\\$[OoTtNnRrVvPp][a-z0-9]*" }), ({ 1 }));
    fmt = fmt[0];
    res = fmt[0];
    for (int i = 1; i < sizeof(fmt); i += 2) {
        int c = fmt[i][1];
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
            num = (c == 't' || c == 'T') ? 1 : 0; // target defaults to 1, not zero
            str = fmt[i][2..<0];
        }
        switch (c) {
        case 'O':
        case 'o':
            obs = args[num];
            bit = conjunction(map(obs, (: $1 ? ($1->is_living() ? ($1 == $(forwhom) ? "you" : $1->query_cap_name()) : $1->query_name()) : 0 :)));
            break;
        case 'T':
        case 't':
            if (str == "") {
                str = "o"; // default to objective
            }
        case 'N':
        case 'n':
            if (str == "") {
                str = "s"; // default is subjective
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
                    bit = who[num]->query_cap_name();
                    break;
                }
            }
            has[who[num]]++;
            bit = who[num]->query_cap_name();
            break;
        case 'R':
        case 'r':
            // @TODO
            break;
        case 'V':
        case 'v':
            if (num >= sizeof(who) || who[num] != forwhom) {
                bit = pluralize(str);
            } else {
                bit = str;
            }
            break;
        case 'P':
        case 'p':
            // @ TODO
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

varargs private mixed *get_soul (string verb, string rule, mixed *args) {
    mapping rules;
    mixed soul;
    mixed *result;
    object *who;
    string tmp;

    if (!(rules = query_emote(verb)) || !(soul = rules[rule])) {
        return 0;
    }

    if (soul[0] == '=') {
        soul = rules[soul[1..]];
    }
    if (!stringp(soul)) {
        soul = map_array(soul, (: $1[<1] == '\n' ? $1 : $1 + "\n" :));
    }

    who = ({ this_character() });
    if (strsrch(rule, "LIV") != -1) {
        int i = 0;
        foreach (string token in explode(rule, " ")) {
            if (token == "LIV") {
                who += ({ args[i] });
                args[i..i] = ({ }); // @TODO why? necessary?
            } else if (token[0] >= 'A' && token[0] <= 'Z') {
                i ++;
            }
        }
    } else if (strsrch(rule, "LVS") != -1) {
        int i = 0;
        foreach (string token in explode(rule, " ")) {
            if (token == "LVS") {
                // @TODO
                who += ({ args[0]... }) - ({ this_character() }); // + args[1];
                i ++;
            } else if (token[0] >= 'A' && token[0] <= 'Z') {
                i ++;
            }
        }
    }
    result = ({ who, allocate(sizeof(who) + 1) });

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

    if (!result[1][<1]) {
        result[1][<1] = compose_message(0, stringp(soul) ? soul : soul[1], who, args...);
    }

    return result;
}

private void display_soul (object *who, string *msgs, mixed others) {
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

int livings_are_remote () {
    return 1;
}

mixed can_verb_rule (mixed args...) {
    string verb, rule;
    mapping emote;
    if (sizeof(args) < 2) return;
    verb = args[0];
    rule = args[1];
    if (!(emote = query_emote(verb))) return;
    return !undefinedp(emote[rule]);
}

mixed direct_verb_rule (mixed args...) {
    string verb, rule;

    if (sizeof(args) < 2) return;
    verb = args[0];
    rule = args[1];

    if (rule == "LVS" && sizeof(args) > 2) {
        object po = previous_object();
        object who = args[2];
        if (po == who || environment(po) != environment(who)) return 0;
    }

    return !undefinedp(query_emote(verb)[rule]);
}
mixed indirect_verb_rule (mixed args...) {
    string verb, rule;
    if (sizeof(args) < 2) return;
    verb = args[0];
    rule = args[1];
    return !undefinedp(query_emote(verb)[rule]);
}

void do_verb_rule (mixed args...) {
    string verb, rule;
    mixed soul;

    if (sizeof(args) < 2) return;
    verb = args[0];
    rule = args[1];

    soul = get_soul(verb, rule, args[2..]);

    if (!soul) {
        return;
    }

    display_soul(soul[0], soul[1], environment(this_character())->query_living_contents() - ({ this_character() }));
}