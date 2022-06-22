// @TODO need to be able to set the beginning
#define THE_BEGINNING   720561600
#define NOW(t)          (t - THE_BEGINNING)

inherit M_CLEAN;

mapping __Astronomy = ([
/*
    "astronomy file": ([
        "SECONDS_PER_MINUTE": integer
        "MINUTES_PER_HOUR": integer
        "HOURS_PER_DAY": integer
        "DAYS_PER_WEEK": integer
        "WEEKS_PER_MONTH": integer
        "MONTHS_PER_YEAR": integer
        "SECOND": integer
        "MINUTE": integer
        "HOUR": integer
        "DAY": integer
        "WEEK": integer
        "MONTH": integer
        "YEAR": integer
        "SHORTEST_DAY": integer
        "LONGEST_DAY": integer
        "TOTAL_DAYS": integer
    ]),
*/
]);

mapping query_astronomy () { return __Astronomy; } // for debug

/* -----  ----- */

private string format_minute (int m) {
    return sprintf("%2'0's", "" + m)[0..1];
}

/* -----  ----- */

int query_now (int t) { return NOW(t); }

int query_second (int t, mapping a) { return NOW(t) % a["MINUTE"]; }
int query_minute (int t, mapping a) { return NOW(t) % a["HOUR"] / a["MINUTE"]; }
int query_hour (int t, mapping a) { return NOW(t) % a["DAY"] / a["HOUR"]; }
int query_day (int t, mapping a) { return NOW(t) % a["WEEK"] / a["DAY"]; }
int query_week (int t, mapping a) { return NOW(t) % a["MONTH"] / a["WEEK"]; }
int query_month (int t, mapping a) { return NOW(t) % a["YEAR"] / a["MONTH"]; }
int query_year (int t, mapping a) { return NOW(t) / a["YEAR"]; }

/* -----  ----- */

mapping query_astronomy_from_room (mixed dest) {
    string path, dirPath;
    string *dirs;

    if (objectp(dest) && dest->is_room()) {
        path = base_name(dest);
    } else if (stringp(dest)) {
        path = dest;
    } else {
        return 0;
    }
    if (!regexp(path, "^/domain/")) return 0;
    dirs = explode(path, "/")[1..<3];

    dirPath = "/domain";
    // @TODO need to do this the reverse direction
    foreach (string dir in dirs) {
        dirPath += "/" + dir;

        // astronomy loaded
        if (__Astronomy[dirPath]) {
            return __Astronomy[dirPath];
        }
    }
    return 0;
}

string query_localtime (mixed dest) {
    mapping a;
    int t;

    if (!(a = query_astronomy_from_room(dest))) return 0;

    t = time();

    return query_hour(t, a) + ":" + format_minute(query_minute(t, a));
}
string query_localdate (mixed dest) {
    mapping a;
    int t;
    string day, month, year;

    if (!(a = query_astronomy_from_room(dest))) return 0;

    t = time();
    day = "" + (query_week(t, a)*(a["WEEK"]/a["DAY"]) + query_day(t, a));
    month = "" + (query_month(t, a) + 1);
    year = "" + query_year(t, a);

    return day + " of Month" + month + " " + year;
}
string query_localsky (mixed dest, string str) {
    mapping a;
    string desc;

    if (!(a = query_astronomy_from_room(dest))) return 0;

    if (str == "sky") {
        if (a["DAY_PHASE"] == "dawn") {
            desc = "%^ORANGE%^It is lit with the colors of a brand new day.%^RESET%^";
        } else if (a["DAY_PHASE"] == "day") {
            desc = "%^YELLOW%^The sun lights up the daytime sky.%^RESET%^";
        } else if (a["DAY_PHASE"] == "dusk") {
            desc = "%^BOLD%^CYAN%^The sun is fading over the western horizon.%^RESET%^";
        } else if (a["DAY_PHASE"] == "night") {
            desc = "%^BOLD%^BLUE%^The sky is darkened with night.%^RESET%^";
        }
    } else  if (str == "sun") {
        if (a["DAY_PHASE"] == "dawn") {
            desc = "%^ORANGE%^The sun is hanging low in the dawning eastern sky.%^RESET%^";
        } else if (a["DAY_PHASE"] == "day") {
            desc = "%^YELLOW%^The sun is shining brightly in the daytime sky.%^RESET%^";
        } else if (a["DAY_PHASE"] == "dusk") {
            desc = "%^BOLD%^CYAN%^The sun is falling into the twilight sky.%^RESET%^";
        }
    }

    return desc;
}

int query_day_of_year (int t, mapping a) {
    int dpw, dpm;
    dpw = a["DAYS_PER_WEEK"];
    dpm = dpw * a["WEEKS_PER_MONTH"];
    return (query_month(t, a) * dpm) + (query_week(t, a) * dpw) + query_day(t, a);
}

/* -----  ----- */

private mixed *format_almanac (int d, string prepend) {
    int hour = to_int((prepend+d)[0..1]);
    int min = to_int((prepend+d)[2..3]) * 60 / 100;
    return ({ hour, min, }); // sprintf("%s:%'0'2s", ""+hour, ""+min) });
}
mapping query_calculate_almanac (int t, mapping a) {
    mapping almanac = ([ ]);
    int days = query_day_of_year(t, a);

    if (days == a["SHORTEST_DAY"]) almanac["equinox"] = "fall";
    else if (days == a["LONGEST_DAY"]) almanac["equinox"] = "spring";
    else if (days == a["LONGEST_DAY"] / 2) almanac["solstice"] = "summer";
    else if (days == a["LONGEST_DAY"] / 2 * 3) almanac["solstice"] = "winter";

    days = (days >= a["LONGEST_DAY"] ? (-1 * days + a["TOTAL_DAYS"]) : days);

    almanac["dawn"] = format_almanac(400 + (2 * days), "0");
    almanac["day"] = format_almanac(500 + (2 * days), "0");
    almanac["dusk"] = format_almanac(2000 - (400 + (2 * days)), "");
    almanac["night"] = format_almanac(2000 - (300 + (2 * days)), "");

    return almanac;
}
void update_almanac (int t, mapping a) {
    a["ALMANAC"] = query_calculate_almanac(t, a);
}
void update_dayphase (int t, mapping a) {
    int h = query_hour(t, a), m = query_minute(t, a);
    int dawnH, dawnM, dayH, dayM;
    int duskH, duskM, nightH, nightM;

    dawnH = a["ALMANAC"]["dawn"][0];
    dawnM = a["ALMANAC"]["dawn"][1];
    dayH = a["ALMANAC"]["day"][0];
    dayM = a["ALMANAC"]["day"][1];
    duskH = a["ALMANAC"]["dusk"][0];
    duskM = a["ALMANAC"]["dusk"][1];
    nightH = a["ALMANAC"]["night"][0];
    nightM = a["ALMANAC"]["night"][1];

    if ((h < dawnH) || (h == dawnH && m < dawnM) || (h >= nightH && m >= nightM)) {
        a["DAY_PHASE"] = "night";
    } else if ((h < dayH) || (h == dayH && m < dayM)) {
        a["DAY_PHASE"] = "dawn";
    } else if ((h < duskH) || (h == duskH && m < duskM)) {
        a["DAY_PHASE"] = "day";
    } else if ((h < nightH) || (h == nightH && m < nightM)) {
        a["DAY_PHASE"] = "dusk";
    }
}

/* -----  ----- */

void handle_room_create (object room) {
    string roomPath, dirPath, aPath;
    string *dirs;

    if (!objectp(room) || !room->is_room()) {
        return;
    }
    roomPath = base_name(room);
    if (!regexp(roomPath, "^/domain/")) return;
    dirs = explode(roomPath, "/")[1..<3];

    dirPath = "/domain";
    foreach (string dir in dirs) {
        dirPath += "/" + dir;

        // astronomy already loaded
        if (__Astronomy[dirPath]) break;

        // check if astronomy exists
        if (file_size(aPath = dirPath + "/astronomy.c") > 0) {
            __Astronomy[dirPath] = aPath->query_astronomy();
            update_almanac(time(), __Astronomy[dirPath]);
            update_dayphase(time(), __Astronomy[dirPath]);
            break;
        }
    }
}

private void process (int t, string key, mapping a) {
    int now, next, total;
    string dayPhase, newPhase;
    int *phase, nextPhase;

    // message("wrap", "%^CYAN%^STD_ASTRONOMY->process%^RESET%^%^: "+t+", "+key+", "+identify(a)+"\n", find_character("diavolo"));

    dayPhase = a["DAY_PHASE"];
    nextPhase = a["NEXT_PHASE"];

    if (nextPhase) {
        if (t >= nextPhase) {
            object *characters = filter(characters(), (: regexp($1->query_environment_path(), "^"+$(key)) :));
            if (dayPhase == "night") {
                dayPhase = "dawn";
                message("astronomy", "%^ORANGE%^The sun appears over the horizon of our reality.%^RESET%^\n", characters);
            } else if (dayPhase == "dawn") {
                dayPhase = "day";
                message("astronomy", "%^YELLOW%^The sun now shines completely on a new day.%^RESET%^\n", characters);
            } else if (dayPhase == "day") {
                dayPhase = "dusk";
                message("astronomy", "%^BOLD%^CYAN%^The sun begins to fall away into twilight.%^RESET%^\n", characters);
            } else if (dayPhase == "dusk") {
                dayPhase = "night";
                message("astronomy", "%^BOLD%^BLUE%^Night darkens the whole of our reality.%^RESET%^\n", characters);
            }
            a["DAY_PHASE"] = dayPhase;
        } else {
            return;
        }
    }

    now = query_hour(t, a) * 100 + query_minute(t, a);

    if (dayPhase == "night") {
        newPhase = "dawn";
        update_almanac(t, a);
    } else if (dayPhase == "dawn") newPhase = "day";
    else if (dayPhase == "day") newPhase = "dusk";
    else if (dayPhase == "dusk") newPhase = "night";

    // message("wrap", "%^CYAN%^STD_ASTRONOMY->process B%^RESET%^%^: newPhase="+newPhase+" a="+identify(a)+"\n", find_character("diavolo"));
    phase = a["ALMANAC"][newPhase];
    next = phase[0] * 100 + phase[1];

    // message("debug", "%^ORANGE%^STD_ASTRONOMY->process%^RESET%^%^: now="+now+", next="+next+"\n", find_character("diavolo"));

    total = (next > now ? next - now : next + (2000 - now)) * 20;
    nextPhase = t + total - (t%a["SECONDS_PER_MINUTE"]);
    a["NEXT_PHASE"] = nextPhase;
    // message("debug", "    %^CYAN%^STD_ASTRONOMY->process B%^RESET%^%^: "+t+", "+dayPhase+" "+nextPhase+", "+next+", "+now+", "+total+"\n", find_character("diavolo"));
}

/* ----- life cycle ----- */

void scan () {
    object *characters = characters();
    // load any existing environments
    if (sizeof(characters)) {
        int t = time();
        map(characters, (: handle_room_create(environment($1)) :));
        // process immediately
        foreach (string key,mapping a in __Astronomy) {
            process(t, key, a);
        }
    }
}

void create () {
    set_no_clean(1);
    set_heart_beat(10);
    if (!clonep()) scan();
}

void heart_beat () {
    int t = time();
    foreach (string key,mapping a in __Astronomy) {
        process(t, key, a);
    }
}