#include <driver/parser_error.h>

#define CFG_PRELOAD "/secure/etc/preload.cfg"

// --- master ------------------------------------------------------------------

/**
 * This apply is called when a new user connects to the driver.
 *
 * @param port The port the user has connected on
 * @returns {STD_USER} the new user object
 */
object connect (int port) {
    object ob;
    string err;

    if (err = catch(ob = new(STD_USER))) {
        if (!this_user()) {
            debug_message(err);
        } else {
            write("Something unexpected happened.\n");
            write(err);
        }
        return 0;
    }
    return ob;
}

// --- startup -----------------------------------------------------------------

/**
 * This apply is called after master has been loaded.
 *
 * @param load_empty
 * @returns returns filenames to preload
 */
varargs string *epilog (int load_empty) {
    string *preload = ({ });
    if (!load_empty && file_size(CFG_PRELOAD) > 0) {
        preload = filter(explode(read_file(CFG_PRELOAD), "\n") - ({ "" }), (: $1[0] != '#' :));
    }
    return preload;
}

/**
 * This apply is called for each driver command line option passed via -f.
 *
 * @param flag a command line option passed to driver
 */
void flag (string flag) {
    if (flag == "test") {
        call_out_walltime((: D_TEST->run(([ "coverage": 1, "shutdown": 1 ])) :), 0.1);
    } else {
        debug_message("master()->flag: received unknown flag.");
    }
}

/**
 * This apply is called for each filename returned by epilog.
 *
 * @param filename the path to load
 */
void preload (string filename) {
    catch (load_object(filename));
}

/**
 * This apply is called by the driver during MSSP requests.
 *
 * @returns key:value pairs of mud stats
 */
mapping get_mud_stats () {
    return ([
        /* --- Required --- */
        "NAME"              : mud_name(),
        "PLAYERS"           : ""+(sizeof(users())),
        "UPTIME"            : ""+(time() - uptime()),

        /* --- Generic --- */
        "CODEBASE"          : mudlib_version(),
        // "CONTACT"           : "",
        "CRAWL DELAY"       : "-1",
        "CREATED"           : "2021",
        // "HOSTNAME"          : "",
        // "ICON"              : "",
        // "IP"                : "",
        // "IPV6"              : "",
        "LANGUAGE"          : "English",
        "LOCATION"          : "US",
        "PORT"              : ""+(driver_port()),
        // "WEBSITE"           : "",

        /* --- Generic --- */
        "FAMILY"            : "LPMud",
        "GAMEPLAY"          : "Player versus Environment",
        "GENRE"             : "Science Fiction",
        "SUBGENRE"          : "Space and Planets",
        "STATUS"            : "Alpha",
        "GAMESYSTEM"        : "Custom",
        // "INTERMUD"          : "",

        /* --- World --- */
        // "AREAS"             : "",
        // "HELPFILES"         : "",
        // "MOBILES"           : "",
        // "OBJECTS"           : "",
        // "ROOMS"             : "",
        // "CLASSES"           : "",
        // "LEVELS"            : "",
        // "RACES"             : "",
        // "SKILLS"            : "",

        /* --- Protocols --- */
        "ANSI"              : "1",
        // "MSP"               : "",
        "UTF-8"             : "1",
        "VT100"             : "0",
        "XTERM 256 COLORS"  : "0",
        "XTERM TRUE COLORS" : "0",

        /* --- Commercial --- */
        "PAY TO PLAY"       : "0",
        "PAY FOR PERKS"     : "U0",

        /* --- Hiring --- */
        "HIRING BUILDERS"   : "0",
        "HIRING CODERS"     : "0",
    ]);
}

// --- build applies -----------------------------------------------------------

/**
 * This apply is called on non-existent files that could be virtually created.
 *
 * @param path file path to check for virtual create
 * @returns loaded object or 0
 */
object compile_object (string path) {
    string area, room, vpath;

    if (!stringp(path) || !sizeof(path)) {
        error("Bad argument 1 to master->compile_object");
    }
    if (path[0..0] != "/") {
        path = "/" + path;
    }

    if (sscanf(path, "/domain/%s/virtual/room/%s/", area, room) == 2) {
        vpath = "/domain/" + area + "/virtual/" + room + ".c";
        if (file_size(vpath) > 0) {
            return call_other(vpath, "virtual_create", path);
        }
    }
    return 0;
}

/**
 * This apply is called by the sprintf efun when printing an object.
 *
 * @param {STD_OBJECT} ob object to name
 * @returns name of the object
 */
string object_name (object ob) {
    if (!ob) {
        return "<destructed>";
    } else if (interactive(ob)) {
        return ob->query_key_name() + " <interactive>";
    }
    return ob->query_short();
}

/**
 * This apply is called to determine directory relative header file paths.
 *
 * @param file path to be checked for additional include paths
 * @returns paths to check for header files
 */
string *get_include_path (string file) {
    string *path = explode(file, "/") - ({ "" }), *paths = ({ ":DEFAULT:" });
    switch (path[0]) {
        case "domain":
            paths += ({ "/domain/" + path[1] + "/include" });
            break;
        case "realm":
            paths += ({ "/realm/" + path[1] + "/include" });
            break;
    }
    return paths;
}

// --- error applies -----------------------------------------------------------

/**
 * This apply is called when shutting down the driver.
 *
 * @param crash_message
 * @param command_giver
 * @param current_object
 */
void crash (string crash_message, object command_giver, object current_object) {
    debug_message(ctime() + " crashed because " + crash_message + " " + identify(call_stack()) + " " + identify(previous_object(-1)));
    message("system", "Everything is suddenly nothing as irreality takes control.\n", users());
    users()->quit_account();
}

string trace_line (object obj, string prog, string file, int line) {
    string objfn = obj ? file_name(obj) : "<none>";
    string ret = objfn;
    int num;
    sscanf(objfn, "%s#%d", objfn, num);
    objfn += ".c";
    if (objfn != prog && objfn != ("/" + prog)) {
        ret += sprintf(" (%s)", prog);
    }
    if (file != prog) {
        ret += sprintf(" at %s:%d\n", file, line);
    } else {
        ret += sprintf(" at line %d\n", line);
    }
    return ret;
}
varargs string standard_trace (mapping e) {
    string ret, t;
    mapping *trace;
    int i, n;

    ret = e["error"] + "Object: " + trace_line(e["object"], e["program"], e["file"], e["line"]) + "\n";
    trace = e["trace"];
    n = sizeof(trace);
    for (i = 0; i < n; i ++) {
        t = trace_line(trace[i]["object"], trace[i]["program"], trace[i]["file"], trace[i]["line"]);
        ret += sprintf("'%s' at %s", trace[i]["function"], t);
    }
    return ret;
}

/**
 * This apply is called to handle caught and runtime errors.
 *
 * @param e error info
 * @param caught flag for runtime vs catch
 */
void error_handler (mapping e, int caught) {
    string ret, file = caught ? "catch" : "runtime";
    /** @type {STD_CHARACTER} */
    object ob;
    /** @type {M_TEST} */
    object test;

    if (caught && sizeof(e["trace"]) > 1 && regexp(e["trace"][1]["program"], D_TEST+"|\\.test\\.c")) {
        test = filter(e["trace"], (: $1["file"] == M_TEST :))[0]["object"];
        if (test && !test->query_expect_catch()) {
            write("--- CAUGHT DURING TEST:\n" + standard_trace(e) + "\n---\n");
        }
        return;
    }

    ret = "--- " + ctime(time()) + "\n" + standard_trace(e) + "\n";
    if (file_size("/log/" + file) > 20000) { // 20 kb
        rename("/log/" + file, "/log/" + file + "-" + time());
    }
    write_file("/log/" + file, ret);
    if ((ob = SEFUN->this_character()) && ob->is_character() && !ob->query_immortal()) {
        D_CHANNEL->send_system("error", ret);
    }
    if (ob = efun::this_player(1)) {
        tell_object(ob, sprintf("%sTrace written to /log/%s\n", e["error"], file));
    }
    return 0;
}

// @TODO: deprecate this disable warnings system
private string *read_file_disabled_warnings (string file) {
    string *lines = file_size(file) > 0 ? explode(read_file(file), "\n") : ({ });
    lines = filter(lines, (: regexp($1,"// disable warning:") :));
    return map_array(lines, (: $1[strsrch($1, ": ")+2..<1] :));
}

/**
 * This apply is called when an error occurs during compilation of a file.
 *
 * @param file path to write to
 * @param msg message to log
 */
void log_error (string file, string msg) {
    string dest, lcMsg, nom, tmp;

    if (file[0] != '/') {
        file = "/" + file;
    }
    if (sscanf(file, "/realm/%s/%s", nom, tmp) == 2) {
        dest = nom;
    } else if (sscanf(file, "/domain/%s/%s", nom, tmp) == 2) {
        dest = "syntax";
    } else if (sscanf(file, "/%s/%s", nom, tmp) == 2) {
        dest = "syntax";
    }
    if (!dest) {
        dest = "log";
    }

    if (regexp(msg, "Warning: ")) {
        if (sizeof(previous_object(-1)) > 1 && base_name(previous_object(-1)[<1]) == D_TEST[0..<3] && previous_object()->query_expect_catch()) {
            return;
        }
        lcMsg = lower_case(msg);
        foreach (string warning in read_file_disabled_warnings(file)) {
            if (regexp(lcMsg, lower_case(warning))) {
                return;
            }
        }
        msg = replace_string(msg, "Warning: ", "\e[33mWarning\e[0m: ", 1);
    } else {
        if (file_size("/log/" + dest) > 20000) { // 20 kb
            rename("/log/" + dest, "/log/" + dest + "-" + time());
        }
        write_file("/log/" + dest, ctime() + " " + msg);
        msg = replace_string(msg, ": ", ": \e[31;1mError\e[0m: ", 1);
    }
    if (msg && this_character() && this_character()->query_immortal()) {
        message("error", msg, this_user());
    } else if (sizeof(previous_object(-1)) > 1 && previous_object(-1)[<1]) {
        if (base_name(previous_object(-1)[<1]) == D_TEST[0..<3]) {
            write(msg + "\n");
        }
    }
}

// --- ed applies --------------------------------------------------------------

/**
 * This apply is called by the ed efun to resolve path names.
 *
 * @param rel_path the relative path to make absolute
 * @returns absolute sanitized path of the relative path
 */
string make_path_absolute (string rel_path) {
    return sanitize_path(rel_path);
}

/**
 * This apply queries the user's ed config.
 *
 * @param {STD_USER} user the user to query
 * @returns the integer bitmask of the user's ed setup
 */
int retrieve_ed_setup (object user) {
    return user->query_ed_setup();
}

/**
 * This apply saves the user's ed config.
 *
 * @param {STD_USER} user the user to save
 * @param config the integer bitmask of the user's ed setup
 * @returns 1
 */
int save_ed_setup (object user, int config) {
    user->set_ed_setup(config);
    return 1;
}

// --- privileges applies ------------------------------------------------------

/**
 * This apply is called when a new file is created to determine privileges
 *
 * @param filename path to check privileges of
 * @returns the path's privileges
 */
string privs_file (string filename) {
    return D_ACCESS->query_file_privs(filename);
}

/**
 * This apply is called whenever database actions are attempted.
 * actions: connect, exec, fetch, close
 *
 * @param ob object requesting database access
 * @param action database action
 * @param info extra info passed by the driver
 * @returns 1
 */
int valid_database (object ob, string action, mixed *info) {
    return 1;
}

/**
 * This apply controls the use of efun:: prefix.
 *
 * @param file the path requesting efun:: prefix
 * @param efun_name the requested efun
 * @param main_file
 * @returns 0 or 1
 */
varargs int valid_override (string file, string efun_name, string main_file) {
    if (file[0] != '/') {
        return 0;
    }
    switch (efun_name) {
        case "input_to":
        case "get_char":
            return regexp(file, "^/std/user/input");
        case "parse_add_rule":
        case "parse_add_synonym":
        case "parse_init":
            return regexp(file, "^/std/module/parse");
    }
    return !!regexp(file, "^/secure/(sefun|daemon/master)");
}

/**
 * This apply is called prior to the use of shadow efun.
 *
 * @param {object} ob the object to be shadowed
 * @returns 0 or 1
 */
int valid_shadow (object ob) {
    // Only mock files or shadow test can use shadows
    if (regexp(file_name(previous_object()), "[a-z]+\\.mock#[0-9]+")) {
        return 1;
    } else if (base_name(ob) == "/std/shadow.test") {
        return 1;
    } else {
        return 0;
    }
}

/**
 * This apply is called prior to every socket efun.
 *
 * @param {object} caller the object requesting a socket
 * @param fn the socket function being requested
 * @param info extra info passed by the driver
 * @returns 0 or 1
 */
int valid_socket (object caller, string fn, mixed *info) {
    return D_ACCESS->query_allowed(caller, fn, 0, "socket");
}

/**
 * This apply is called for each of the read efuns.
 *
 * Read efuns:
 *    file_size, get_dir, include, load_object, read_bytes, read_file,
 *    restore_object, stat,
 * Read/write efuns:
 *    ed_start, compress_file, move_file,
 *
 * @param file the path being read
 * @param caller the source of the efun call
 * @param fn the efun being called
 * @returns 0 or 1
 */
int valid_read (string file, mixed caller, string fn) {
    int valid = 0;
    if (stringp(file) && sizeof(file)) {
        file = sanitize_path(file);
        if (!(valid = regexp(base_name(caller), "^/secure/daemon/[master|access]"))) {
            valid = D_ACCESS->query_allowed(caller, fn, file, "read");
        }
        if (!valid && !regexp(base_name(previous_object()), "\\.test$")) {
            debug_message(ctime()+" "+base_name(caller)+" denied read ("+fn+") to "+file);
        }
    }
    return valid;
}

/**
 * This apply is called for each of the write efuns.
 *
 * Read/write efuns:
 *    ed_start, compress_file, move_file,
 * Write efuns:
 *    debugmalloc, dumpallobj, mkdir, remove_file, rename, rmdir,
 *    save_object, sqlite3_connect, trace_start, write_bytes, write_file,
 *
 * @param file the path being read
 * @param caller the source of the efun call
 * @param fn the efun being called
 * @returns 0 or 1
 */
int valid_write (string file, mixed caller, string fn) {
    int valid = 0;
    file = sanitize_path(file);
    if (!(valid = regexp(base_name(caller), "^/secure/daemon/[master|access]"))) {
        valid = D_ACCESS->query_allowed(caller, fn, file, "write");
    }
    if (!valid && !regexp(base_name(previous_object()), "\\.test$")) {
        debug_message(ctime()+" "+base_name(caller)+" denied write ("+fn+") to "+file);
    }
    return valid;
}

// --- parser applies ----------------------------------------------------------

/**
 * List of nouns that apply to all objects.
 *
 * @returns string array containing base identifiers
 */
string *parse_command_id_list () {
    return ({ "thing" });
}

/**
 * List of adjectives that apply to all objects.
 *
 * @returns string array containing adjectives
 */
string *parse_command_adjectiv_id_list () {
    return ({ "a", "an", "the" });
}

/**
 * List of plurals that apply to all objects.
 *
 * @returns string array containing plural identifiers
 */
string *parse_command_plural_id_list () {
    return ({ "things", "them", "everything" });
}

/**
 * List of prepositions that are permitted.
 *
 * @returns string array containing prepositions
 */
string *parse_command_prepos_list () {
    return ({ "in", "from", "on", "under", "behind", "beside", "of", "for",
        "to", "with", "at", "off", "out", "down", "up", "around", "over",
        "into", "onto", "about", "out of", "aboard", "above", "against",
        "alongside", "beneath", "below", "besides", "by", "inside", "outside",
        "through", "underneath", "upon", "within", "without", "alongside of",
        "back of", "down from", "inside of", "round about", "near to",
        "next to", "over to", "outside of", "up to", "in front of",
        "in back of", "on top of", "off of" });
}

/**
 * A word that refers to everything in an environment
 *
 * @returns string "all" word
 */
string parse_command_all_word () {
    return "all";
}

/**
 * A list of objects that can match remote living objects.
 *
 * @returns {STD_CHARACTER} list of characters
 */
object *parse_command_users () {
    return users()->query_character();
}

/**
 * This apply is called to generate error responses to user input.
 *
 * @param type the type of parser error message
 * @param {STD_OBJECT} ob the object in context
 * @param arg
 * @param plural
 */
string parser_error_message (int type, object ob, mixed arg, int plural) {
    string err;
    object tmpob;
    string wut;
    /** @type {STD_OBJECT} */
    object wat;

    if (ob) {
        err = ob->query_short();
    } else {
        err = "";
    }

    if (sizeof(arg) && arg[<1] == '\n') {
        // trim newline added by driver
        arg = arg[0..<2];
    }

    debug_message("parser_error_message: "+type+" "+identify(ob)+" "+arg+" "+plural);
    switch (type) {
    case 0:
        if (arg && objectp(arg)) {
            wat = arg;
        }
        if (!wat && arg && arrayp(arg) && sizeof(arg)) {
            foreach (mixed element in arg) {
                if (objectp(element)) {
                    wat = element;
                }
            }
        }
        if (ob && wat) {
            err = "You can't use " + ob->query_short() + " with " + wat->query_short() + " that way.";
        } else if (ob) {
            err = "It seems you can't do that with " + ob->query_short() + ".";
        } else if (wat) {
            err = "It seems you can't do that to " + wat->query_short() + ".";
        } else {
            err = "It seems you can't do that.";
        }
        break;

    case ERR_IS_NOT:
        if (plural || (arg && stringp(arg))) {
            if (plural) { // || get_object(arg, this_player())) {
                return "It appears you must be more specific.";
            } else if (arg && stringp(arg)) {
                wut = remove_article(arg);
            }
        } else {
            wut = "that";
        }
        err = capitalize(wut) +" is not here.";
        break;

    case ERR_NOT_LIVING:
        if (plural) {
            err = "None of the " + pluralize(remove_article(arg)) +" are alive.";
        } else {
            err = "The " + remove_article(arg) + " is not alive.";
        }
        break;

    case ERR_NOT_ACCESSIBLE:
        if (plural) {
            err = "You can't get to them.";
        } else {
            err = "You can't get to it.";
        }
        break;

    case ERR_AMBIG:
        {
            mixed *obs;
            int i;

            obs = unique_array(arg, (: $1->query_short() :));
            if (sizeof(obs) == 1) {
                err = "Which of the " + consolidate(sizeof(arg), obs[0][0]->query_short()) + " do you mean?";
            } else {
                err = "Do you mean ";
                for (i = 0; i < sizeof(obs); i ++) {
                    if (sizeof(obs[i]) > 1) {
                        err += "one of the " + consolidate(sizeof(obs[i]), obs[i][0]->query_short());
                    } else {
                        err += obs[i][0]->query_short();
                    }
                    if (i == (sizeof(obs) - 2)) {
                        err += " or ";
                    } else if (i < sizeof(obs) - 1) {
                        err += ", ";
                    }
                }
                err += "?";
            }
            return err;
        }

    case ERR_ORDINAL:
        if (arg > 1) {
            err = "There are only " + arg + " of them.";
        } else {
            err = "There is only one of them.";
        }
        break;

    case ERR_ALLOCATED:
        return arg;

    case ERR_THERE_IS_NO:
        if (plural || (arg && stringp(arg)) && environment(this_character())) {
            // ???
            if (tmpob = present(arg, environment(this_character()))) {
                return "It seems you must be more specific.";
            } else if (arg && stringp(arg)) {
                wut = remove_article(arg);
            }
        } else {
            wut = "such thing";
        }
        err = "There is no "+ wut +" here.";
        break;

    case ERR_BAD_MULTIPLE:
        err = "You can't do that to more than one at a time.";
        break;
    }
    return err;
}

/**
 * This function is called whenever characters enter or exit the world.
 */
void handle_parse_refresh () {
    parse_refresh();
}

// --- object applies ----------------------------------------------------------

/**
 * Upon create, if this is not a clone, attempt to make save directories.
 */
void create () {
    int i;
    string s = " ";
    if (clonep()) {
        return;
    }
    for (i = 'a'; i <= 'z'; i ++) {
        s[0] = i;
        mkdirs("/save/account/" + s);
        mkdirs("/save/character/" + s);
        mkdirs("/save/planet/" + s);
    }
    for (i = 'A'; i <= 'Z'; i ++) {
        s[0] = i;
        mkdirs("/save/account/" + s);
    }
    for (i = 0; i <= 9; i ++) {
        s = "" + i;
        mkdirs("/save/account/" + s);
    }
}