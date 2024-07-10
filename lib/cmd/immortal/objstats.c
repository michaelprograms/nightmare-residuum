/*
_Type___   _Blueprints__   _Clones______   _Dangling____   _Total_______
Abcdef     #####  ##.##%   #####  ##.##%   #####  ##.##%   #####  ##.##%
*/
void format_line(string type, int files, int clones, int leaks, int num, int totalAll) {
    string result = sprintf("%8s", type) + "   ";
    result += sprintf("%5d  %5.2f", files, (files * 100.0 / totalAll)) + "%   ";
    if (clones > -1) result += sprintf("%5d  %5.2f", clones, (clones * 100.0 / totalAll)) + "%   ";
    else result += "                ";
    if (leaks > -1) result += sprintf("%5d  %5.2f", leaks, (leaks * 100.0 / totalAll)) + "%   ";
    else result += "                ";
    result += sprintf("%5d %6.2f", num, (num * 100.0 / totalAll)) + "%";
    write(result + "\n");
}

void command (string input, mapping flags) {
    // grab start time
    int t = rusage()["utime"] + rusage()["stime"];

    object *livs = filter(objects(), (:$1 && $1->is_living():));
    object *rooms = filter(objects(), (:$1 && $1->is_room():));
    object *obs = objects() - livs - rooms;
    int totalObs = sizeof(obs);
    int totalRooms = sizeof(rooms);
    int totalLivs = sizeof(livs);
    int totalAll = totalObs + totalRooms + totalLivs;
    int nObsLoaded = sizeof(filter(obs, (:$1 && !environment($1) && !regexp(file_name($1), "#"):)));
    int nObsCloned = sizeof(filter(obs, (:$1 && environment($1) && regexp(file_name($1), "#"):)));
    object *obsLeaked = filter(obs, (:$1 && !environment($1) && regexp(file_name($1), "#") && base_name($1) != "/secure/std/login":)); // exclude login object
    int nLivsLoaded = sizeof(filter(livs, (:$1 && !environment($1) && !regexp(file_name($1), "#"):)));
    int nLivsCloned = sizeof(filter(livs, (:$1 && environment($1) && regexp(file_name($1), "#"):)));
    object *livsLeaked = filter(livs, (:$1 && !environment($1) && regexp(file_name($1), "#"):));
    int nRoomsLoaded = sizeof(filter(rooms, (:$1 && !regexp(file_name($1), "#"):)));
    object *roomsCloned = filter(rooms, (:$1 && regexp(file_name($1), "#"):));
    int nRoomsCloned = sizeof(roomsCloned);

    write(" Type       Blueprints      Clones          Leaked          Total       \n");
    write("--------   -------------   -------------   -------------   -------------\n");
    format_line("Objects", nObsLoaded, nObsCloned, sizeof(obsLeaked), totalObs, totalAll);
    format_line("Livings", nLivsLoaded, nLivsCloned, sizeof(livsLeaked), totalLivs, totalAll);
    format_line("Rooms", nRoomsLoaded, nRoomsCloned, -1, totalRooms, totalAll);
    write("--------   -------------   -------------   -------------   -------------\n");
    format_line("Total", nObsLoaded + nLivsLoaded + nRoomsLoaded, nObsCloned + nLivsCloned + nRoomsCloned, -1, totalObs + totalLivs + totalRooms, totalAll);

    if (input == "objects" && sizeof(obsLeaked) > 0) {
        write("\nDangling objects:\n");
        for (int i = 0; i < sizeof(obsLeaked); i ++) {
            // object->create sets "spawned_by" property containing previous_object(-1) list
            message("no wrap", sprintf("%-4s%O", ""+i, obsLeaked[i]), this_user());
        }
    }
    if (input == "livings" && sizeof(livsLeaked) > 0) {
        write("\nDangling livings:");
        for (int i = 0; i < sizeof(livsLeaked); i ++) {
            write(sprintf("%-4s%O", ""+i, livsLeaked[i]));
        }
    }
    if (input == "rooms" && sizeof(roomsCloned) > 0) {
        write("\nSpawned rooms:");
        for (int i = 0; i < sizeof(roomsCloned); i ++) {
            write(sprintf("%-4s", ""+i) + identify(roomsCloned[i]));
        }
    }

    // calculate total time
    t = rusage()["utime"] + rusage()["stime"] - t;
    write("Done! (%^ORANGE%^"+t+" ms%^RESET%^)");
}

void help() {
    write(" Syntax  : "+format_syntax("<objstats (objects|livings|rooms)>")+
        "\n\n"+
        "With no argument, shows the breakdown of total objects, broken down "+
        "by Type (Objects/Livings/Rooms), Blueprints (loaded into memory), "+
        "Clones (spawned copies), and Leaked (spawned with no environment)."+
        "\n\n"
        "If provided with an argument, will also print the list of leaked "+
        "objects, leaked livings, or spawned rooms."
        "\n"
    );
}