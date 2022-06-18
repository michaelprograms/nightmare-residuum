inherit M_CLEAN;

mapping __Astronomy = ([
/*
    "astronomy file": object,
*/
]);

mapping query_astronomy () {
    return __Astronomy;
}

string query_astronomy_from_room (mixed dest) {
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
            return dirPath;
        }
    }
    return 0;
}

string query_localtime (mixed dest) {
    string path = query_astronomy_from_room(dest);
    if (!path) return 0;
    return __Astronomy[path]->query_time_of_day();
}
string query_localdate (mixed dest) {
    string path = query_astronomy_from_room(dest);
    if (!path) return 0;
    return __Astronomy[path]->query_datetime_of_year();
}

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
    // @TODO need to do this the reverse direction
    foreach (string dir in dirs) {
        dirPath += "/" + dir;

        // astronomy already loaded
        if (__Astronomy[dirPath]) break;

        // check if astronomy exists
        if (file_size(aPath = dirPath + "/astronomy.c") > 0) {
            __Astronomy[dirPath] = clone_object(aPath);
            break;
        }
    }
}

void process (int t, string key, object ob) {
    int now, next, total;
    string dayPhase, newPhase;
    int *phase, nextPhase;

    dayPhase = ob->query_day_phase();
    nextPhase = ob->query_next_phase();

    if (nextPhase) {
        if (t >= nextPhase) {
            string path = split_path(base_name(ob))[0];
            object *characters = filter(characters(), (: regexp($1->query_environment_path(), "^"+$(path)) :));
            if (dayPhase == "night") {
                dayPhase = ob->set_day_phase("dawn");
                message("astronomy", "%^ORANGE%^The sun appears over the horizon of our reality.%^RESET%^\n", characters);
            } else if (dayPhase == "dawn") {
                dayPhase = ob->set_day_phase("day");
                message("astronomy", "%^YELLOW%^The sun now shines completely on a new day.%^RESET%^\n", characters);
            } else if (dayPhase == "day") {
                dayPhase = ob->set_day_phase("dusk");
                message("astronomy", "%^BOLD%^CYAN%^The sun begins to fall away into twilight.%^RESET%^\n", characters);
            } else if (dayPhase == "dusk") {
                dayPhase = ob->set_day_phase("night");
                message("astronomy", "%^BOLD%^BLUE%^Night darkens the whole of our reality.%^RESET%^\n", characters);
            }
        } else {
            message("debug", "next astronomy change in " + (nextPhase - t)+"\n", find_character("diavolo"));
            return;
        }
    }

    now = ob->query_hour(t) * 60 + ob->query_minute(t);

    if (dayPhase == "night") {
        newPhase = "dawn";
        ob->update_almanac(t);
    } else if (dayPhase == "dawn") newPhase = "day";
    else if (dayPhase == "day") newPhase = "dusk";
    else if (dayPhase == "dusk") newPhase = "night";

    // message("debug", "    %^CYAN%^STD_ASTRONOMY->process B%^RESET%^%^: "+t+", "+next+", "+now+"\n", find_character("diavolo"));
    phase = ob->query_almanac()[newPhase];
    next = phase[0] * 60 + phase[1];

    total = (next > now ? next - now : next + (1200 - now)) * 20;
    nextPhase = t + total;
    ob->set_next_phase(nextPhase);
    // message("debug", "    %^CYAN%^STD_ASTRONOMY->process B%^RESET%^%^: "+t+", "+dayPhase+" "+nextPhase+", "+next+", "+now+", "+total+"\n", find_character("diavolo"));
}

/* ----- life cycle ----- */

void create () {
    object *characters;
    set_no_clean(1);
    set_heart_beat(10);

    // load any existing environments
    if (sizeof(characters = characters())) {
        map(characters, (: handle_room_create(environment($1)) :));
    }
}

int handle_remove () {
    foreach (string key,object ob in __Astronomy) {
        ob->handle_remove();
    }
    return ::handle_remove();
}

void heart_beat () {
    int t = time();
    foreach (string key,object ob in __Astronomy) {
        process(t, key, ob);
    }
}