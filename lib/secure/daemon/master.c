#include "/secure/include/driver/parser_error.h"
#include "/secure/include/global.h"
#include "/secure/sefun/sefun.h"

#define CFG_PRELOAD "/secure/etc/preload.cfg"

// --- master ------------------------------------------------------------------

// This apply is called when a new user connects to the driver.
object connect (int port) {
    object ob;
    string err;

    if (err = catch(ob = clone_object(OBJ_USER))) {
        if (!this_player()) {
            debug_message(err);
        } else {
            write("Something unexpected happened.\n");
            write(err);
        }
        return 0;
    }
    return ob;
}

// This apply is called when a new file is created to determine privilege
string privs_file (string filename) {
    return D_ACCESS->query_file_privs(filename);
}

// --- startup -----------------------------------------------------------------

// This apply is called after master has been loaded.
// Returns an array of filenames to preload.
varargs string *epilog (int load_empty) {
    string *preload = ({ });
    if (!load_empty && file_size(CFG_PRELOAD) > 0) {
        preload = filter(explode(read_file(CFG_PRELOAD), "\n") - ({ "" }), (: $1[0] != '#' :));
    }
    return preload;
}

// This apply is called for each driver command line option passed via -f.
void flag (string flag) {
    if (flag == "test") {
        call_out((: D_TEST->run(1) :), 0);
    } else {
        debug_message("master()->flag: received unknown flag.");
    }
}

// This apply is called for each filename returned by epilog.
void preload (string filename) {
    catch (load_object(filename));
}

// This apply is called by the driver during MSSP requests.
mapping get_mud_stats () {
    return ([
        /* --- Required --- */
        "NAME"              : mud_name(),
        "PLAYERS"           : ""+(sizeof(users())),
        // "UPTIME"            : ""+(time() - uptime()),

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

// This apply is called on non-existant files that could be virtually created.
object compile_object (string path) {
    object ob;
    string area, room, vpath;

    if (!path) return 0;
    if (path[0..0] != "/") path = "/" + path;

    if (sscanf(path, "/domain/%s/virtual/room/%s/", area, room)) {
        vpath = sprintf("%s/%s/virtual/%s", "/domain", area, room);
        if (file_size(vpath + ".c") >= 0) {
            if (ob = vpath->virtual_create(path)) {
                return ob;
            }
        }
    }
    return 0;
}

// This apply is called by the sprintf efun when printing an object.
string object_name (object ob) {
    if (!ob) return "<destructed>";
    else if (interactive(ob)) return ob->query_key_name() + " <interactive>";
    return 0;
}

// This apply is called to determine directory relative header file paths.
string *get_include_path (string file) {
    string *path = explode(file, "/"), *paths = ({ ":DEFAULT:" });
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

// This apply is called when shutting down the driver.
void crash (string crash_message, object command_giver, object current_object) {
    debug_message(ctime() + " crashed because " + crash_message + " " + identify(command_stack()) + " " + identify(caller_stack()));
    message("system", "Everything is suddenly nothing as irreality takes control.\n", users());
    users()->quit_account();
}

private string trace_line (object obj, string prog, string file, int line) {
    string objfn = obj ? program_name(obj)[0..<3] : "<none>";
    string ret = objfn;
    int num;
    sscanf(objfn, "%s#%d", objfn, num);
    objfn += ".c";
    if (objfn != prog && objfn != ("/" + prog)) ret += sprintf(" (%s)", prog);
    if (file != prog) ret += sprintf(" at %s:%d\n", file, line);
    else ret += sprintf(" at line %d\n", line);
    return ret;
}
private varargs string standard_trace (mapping e, int flag) {
    string ret, t;
    mapping *trace;
    int i, n;

    ret = e["error"] + "Object: " + trace_line(e["object"], e["program"], e["file"], e["line"]) + "\n";
    trace = e["trace"];
    n = sizeof(trace);
    for (i = 0; i < n; i ++) {
        if(flag) ret += sprintf("#%d: ", i);
        t = trace_line(trace[i]["object"], trace[i]["program"], trace[i]["file"], trace[i]["line"]);
        ret += sprintf("'%s' at %s", trace[i]["function"], t);
    }
    return ret;
}

// This apply is called to handle caught and runtime errors.
void error_handler (mapping e, int caught) {
    string ret, file = caught ? "catch" : "runtime";

    if (caught && sizeof(e["trace"]) > 1 && e["trace"][1]["program"] == D_TEST) {
        object test = filter(e["trace"], (: $1["file"] == "/secure/module/test" :))[0]["object"];
        if (test && !test->query_expect_catch()) {
            write("--- CAUGHT DURING TEST:\n" + standard_trace(e) + "\n---\n");
        }
        return;
    }

    ret = "--- " + ctime(time()) + "\n" + standard_trace(e) + "\n";
    if (file_size("/log/"+file) > 20000) { // 20 kb
        rename("/log/"+file, "/log/"+file+"-"+time());
    }
    write_file("/log/"+file, ret);
    // @TODO CHAT_D->do_chat("runtime", ret , 2 , 0);
    if (this_player()) {
        tell_object(this_player(), sprintf("%sTrace written to /log/%s\n", e["error"], (caught ? "catch" : "runtime")));
    }
    return 0;
}

private string *read_file_disabled_warnings (string file) {
    string *lines = file_size(file) > 0 ? explode(read_file(file), "\n") : ({});
    lines = filter(lines, (: sizeof(regexp(({ $1 }),"// disable warning:")) :));
    return map(lines, (: $1[strstr($1, ": ")+2..<1] :));
}

// This apply is called when an error occurs during compilation of a file.
void log_error (string file, string msg) {
    string dest, lcMsg, nom, tmp;

    debug_message("log_error: "+file+" "+msg+"\n");
    if (file[0] != '/') {
        file = "/" + file;
    }
    if (sscanf(file, "/realm/%s/%s", nom, tmp) == 2) {
        dest = nom;
    } else if (sscanf(file, "/domain/%s/%s", nom, tmp) == 2) {
        dest = nom;
    } else if (sscanf(file, "/%s/%s", nom, tmp) == 2) {
        dest = nom;
    }
    if (!dest) {
        dest = "log";
    }

    if (sizeof(regexp(({ msg }), "Warning: "))) {
        if (sizeof(caller_stack()) > 1 && object_name(caller_stack()[<1]) == D_TEST[0..<3] && previous_object()->query_expect_catch()) {
            return;
        }
        lcMsg = lower_case(msg);
        foreach (string warning in read_file_disabled_warnings(file)) {
            if (sizeof(regexp(({ lcMsg }), lower_case(warning)))) {
                return;
            }
        }
        msg = regreplace(msg, "Warning: ", "\e[33mWarning\e[0m: ", 1);
    } else {
        if (file_size("/log/" + dest) > 20000) { // 20 kb
            rename("/log/" + dest, "/log/" + dest + "-" + time());
        }
        write_file("/log/" + dest, ctime() + " " + msg);
        msg = regreplace(msg, ": ", ": \e[31;1mError\e[0m: ", 1);
    }
    if (msg && SEFUN->this_character() && SEFUN->this_character()->query_immortal()) {
        message("wrap", msg, this_player());
    } else if (sizeof(caller_stack()) > 1 && caller_stack()[<1]) {
        if (object_name(caller_stack()[<1]) == D_TEST[0..<3]) {
            write(msg + "\n");
        }
    }
}

// --- ed applies --------------------------------------------------------------

// get_save_file_name
/*
    This  master apply is called by ed() when a player disconnects while in
    the editor and editing a file.  This function should return  an  alternate
    file name for the file to be saved, to avoid overwriting the original.

    string get_save_file_name(string filename?);
*/

// This apply is called by the ed efun to resolve path names.
string make_path_absolute (string rel_path) {
    return SEFUN->sanitize_path(rel_path);
}

// retrieve_ed_setup
/*
    This master apply is called by the ed() efun to retrieve  a  user's  ed
    setup/configuration  settings.   This  function should return the setup
    (contained in an int).

    int retrieve_ed_setup(object user);
    user->query_ed()
*/

// save_ed_setup
/*
    This  master  apply is called by the ed() efun to save a user's ed setup/
    configuration settings (contained in an int).  This function  should
    return an int for success (1 or TRUE)/failure (0 or FALSE).

    int save_ed_setup(object user, int config);
    user->set_ed(config)
*/

// --- valid applies -----------------------------------------------------------

// valid_bind

// valid_database

// valid_hide

// valid_link

// valid_object

// This apply controls the use of efun:: prefix.
varargs int valid_override (string file, string efun_name, string main_file) {
    if (file[0] != '/') return 0;

    switch (efun_name) {
        case "input_to":
        case "get_char":
            return file == "/secure/user/input";
        case "clone_object":
            return file == "/std/module/test";
    }
    return sizeof(regexp(({ file }), "/secure/sefun"));
}


// valid_save_binary

// valid_seteuid

// valid_shadow

// This apply is called prior to every socket efun.
int valid_socket (object caller, string fn, mixed *info) {
    int valid = 0;
    if (sizeof(regexp(({ program_name(caller)[0..<3] }), "/secure/daemon/ipc"))) valid = 1;
    else if (sizeof(regexp(({ program_name(caller)[0..<3] }), "/cmd/immortal/cat"))) valid = 1;
    // @TODO D_ACCESS->query_allowed(caller, fn, 0, "socket")
    return valid;
}

/*
    Read efuns:
        file_size, get_dir, include, load_object, read_bytes, read_file,
        restore_object, stat,
    Read/write efuns:
        ed_start, compress_file, move_file,
    Write efuns:
        debugmalloc, dumpallobj, mkdir, remove_file, rename, rmdir,
        save_object, sqlite3_connect, trace_start, write_bytes, write_file,
*/
// This apply is called for each of the read efuns
int valid_read (string file, mixed uid, string fn, mixed caller) {
    int valid = 0;
    file = SEFUN->sanitize_path(file);

    if (!(valid = sizeof(regexp(({ program_name(caller) }), "/secure/[master|daemon/master|daemon/access]")))) {
        valid = D_ACCESS->query_allowed(caller, fn, file, "read");
    }
    if (!valid && !sizeof(regexp(({ program_name(previous_object()) }), "\\.test$"))) {
        debug_message(ctime()+" "+program_name(caller)+" denied read ("+fn+") to "+file);
    }
    return valid;
}
// This apply is called for each of the write efuns
int valid_write (string file, mixed uid, string fn, mixed caller) {
    int valid = 0;
    file = SEFUN->sanitize_path(file);

    if (!(valid = sizeof(regexp(({ program_name(caller) }), "/secure/[master|daemon/master|daemon/access]")))) {
        valid = D_ACCESS->query_allowed(caller, fn, file, "write");
    }
    if (!valid && !sizeof(regexp(({ program_name(previous_object()) }), "\\.test$"))) {
        debug_message(ctime()+" "+program_name(caller)+" denied write ("+fn+") to "+file);
    }
    return valid;
}

// -----------------------------------------------------------------------------

// List of nouns that apply to all objects
string *parse_command_id_list () {
    return ({ "thing" });
}
// List of adjectives that apply to all objects
string *parse_command_adjectiv_id_list () {
    return ({ "a", "an", "the" });
}
// List of plurals that apply to all objects
string *parse_command_plural_id_list () {
    return ({ "things", "them", "everything" });
}
// List of prepositions that are permitted
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
// A word that refers to everything in an environment
string parse_command_all_word () {
    return "all";
}
// A list of objects that can match remote living objects.
object *parse_command_users () {
    return users()->query_character();
}
// This apply is called to generate error responses to user input.
string parser_error_message (int type, object ob, mixed arg, int plural) {
    string err;
    object tmpob;
    string wut;
    object wat;

    if (ob) err = ob->query_short();
    else err = "";

    debug_message("parser_error_message: "+type+" "+identify(ob || "no ob")+" "+arg+" "+plural);
    switch (type) {
    case 0:
        if (arg && objectp(arg)) wat = arg;
        if (!wat && arg && pointerp(arg) && sizeof(arg)) {
            foreach (mixed element in arg) {
                if (objectp(element)) wat = element;
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
            } else if (arg && stringp(arg)) wut = remove_article(arg);
        } else wut = "that";
        err = capitalize(wut) +" is not here.";
        break;

    case ERR_NOT_LIVING:
        if (plural) err = "None of the " + pluralize(remove_article(arg)) +" are alive.";
        else err = "The " + remove_article(arg) + " is not alive.";
        break;

    case ERR_NOT_ACCESSIBLE:
        if (plural) err = "You can't get to them.";
        else err = "You can't get to it.";
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
                        err += "one of the " + consolidate(sizeof(obs[i]),obs[i][0]->query_short());
                    } else err += obs[i][0]->query_short();
                    if (i == (sizeof(obs) - 2)) err += " or ";
                    else if (i < sizeof(obs) - 1) err += ", ";
                }
                err += "?";
            }
            return err;
        }

    case ERR_ORDINAL:
        if (arg > 1) err = "There are only " + arg + " of them.";
        else err = "There is only one of them.";
        break;

    case ERR_ALLOCATED:
        return arg;

    case ERR_THERE_IS_NO:
        if (plural || (arg && stringp(arg)) && environment(SEFUN->this_character())) {
            // ???
            if (tmpob = present(arg, environment(SEFUN->this_character()))) {
                return "It seems you must be more specific.";
            } else if (arg && stringp(arg)) wut = remove_article(arg);
        } else wut = "such thing";
        err = "There is no "+ wut +" here.";
        break;

    case ERR_BAD_MULTIPLE:
        err = "You can't do that to more than one at a time.";
        break;
    }
    return err;
}

// This function is called whenever characters enter or exit the world.
void handle_parse_refresh () {
    // parse_refresh();
}