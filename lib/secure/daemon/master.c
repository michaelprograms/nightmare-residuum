#include <driver/parser_error.h>

#define CFG_PRELOAD "/secure/etc/preload.cfg"

// --- master ------------------------------------------------------------------

object connect (int port) {
    object ob;
    string err;

    if (err = catch(ob = clone_object(OBJ_USER))) {
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

// privs_file
/*
    The privs_file() function is called in the master  object  when  a  new
    file  is  created.  The 'filename' of the object is passed as the argu‐
    ment, and the string that privs_file()  returns  is  used  as  the  new
    object's privs string.

    The  privs_file() functionality is only available if the driver is com‐
    piled with the PRIVS option defined.

    string privs_file(string filename);
*/
string privs_file (string filename) {
    return D_ACCESS->query_file_privs(filename);
}

// --- startup -----------------------------------------------------------------

// This apply is called after master has been loaded.
// Returns an array of filenames to preload.
string *epilog (int load_empty) {
    string *preload = ({});
    if (!load_empty && file_size(CFG_PRELOAD) > 0) {
        preload = filter_array(explode(read_file(CFG_PRELOAD), "\n") - ({ "" }), (: $1[0] != '#' :));
    }
    return preload;
}

// This apply is called for each command line option passed to the driver with the -f flag.
void flag (string flag) {
    if (flag == "test") {
        // catch("/secure/test/tests.c"->run());
        call_out(function() { D_TEST->run(); }, 0);
    } else {
        debug_message("master()->flag: received unknown flag.");
    }
}

// This apply is called for each filename returned by epilog.
void preload (string filename) {
    catch(load_object(filename));
}

mapping get_mud_stats () {
    return ([
        /* --- Required --- */
        "NAME"              : mud_name(),
        "PLAYERS"           : ""+(sizeof(users())),
        "UPTIME"            : ""+(time() - uptime()),

        /* --- Generic --- */
        "CODEBASE"          : mudlib_version(),
        // "CONTACT"           : "@TODO",
        "CRAWL DELAY"       : "-1",
        "CREATED"           : "2021",
        // "HOSTNAME"          : "@TODO",
        // "ICON"              : "@TODO",
        // "IP"                : "",
        // "IPV6"              : "",
        "LANGUAGE"          : "English",
        "LOCATION"          : "US",
        "PORT"              : ""+(driver_port()),
        // "WEBSITE"           : "@TODO",

        /* --- Generic --- */
        "FAMILY"            : "LPMud",
        "GAMEPLAY"          : "Player versus Environment",
        "GENRE"             : "Science Fiction",
        "SUBGENRE"          : "Space and Planets",
        "STATUS"            : "Alpha",
        "GAMESYSTEM"        : "Custom",
        // "INTERMUD"          : "",

        /* --- World --- */
        // "AREAS"             : "@TODO",
        // "HELPFILES"         : "@TODO",
        // "MOBILES"           : "@TODO",
        // "OBJECTS"           : "@TODO",
        // "ROOMS"             : "@TODO",
        // "CLASSES"           : "@TODO",
        // "LEVELS"            : "@TODO",
        // "RACES"             : "@TODO",
        // "SKILLS"            : "@TODO",

        /* --- Protocols --- */
        "ANSI"              : "1",
        // "MSP"               : "@TODO",
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

// compile_object
/*
    The driver  calls  compile_object(3)  in  the  event  that  the  mudlib
    instructs  the driver to load a file that does not exist.  For example,
    the driver will call compile_object("/obj/file.r")  in  master  if  the
    mudlib     calls    call_other("/obj/file.r",    "some_function")    or
    new("/obj/file.r") and /obj/file.r.c names a file that does not  exist.
    The  compile_object()  function  is  expected to return 0 if the mudlib
    does not wish to associate an object with the file name  "/obj/file.r".
    If  the  mudlib  does  wish  to  associate  an object with the filename
    "/obj/file.r", then the mudlib should return the object it wishes asso‐
    ciated.  After an association is made between an object and a filename,
    then it will be as if the file, file.r.c, did exist (to the driver) and
    when loaded produced the object that compile_object() returned.

    object compile_object(string pathname);
*/
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

// object_name
/*
    This  master  apply  is called by the sprintf() efun, when printing the
    "value" of an object.  This function should return a string correspond‐
    ing to the name of the object (eg a user's name).

    string object_name(object ob);
*/
string object_name (object ob) {
    if (!ob) return "<destructed>";
    else if (interactive(ob)) return ob->query_key_name() + " <interactive>";
    return 0;
}

// Add directory relative header files.
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

// Shutdown before driver crashes from segmentation fault or other error.
void crash (string crash_message, object command_giver, object current_object) {
    // string guilty_stack = get_stack(), guilty_obs = identify(previous_object(-1));
    // write_file(DIR_LOGS "/crashes", mud_name()+" crashed "+ctime(time())+" with error "+crash_message+".\n"+guilty_stack+"\n"+guilty_obs+"\n---\n");
    // write_file(DIR_LOGS+"/crashes", mud_name()+" crashed "+ctime(time())+" with error "+
    // crash_message+".\n");
    // if(this_player()) write_file(DIR_LOGS+"/crashes", identify(this_player())+"\n");
    // if(query_verb()) write_file(DIR_LOGS+"/crashes", " verb "+query_verb()+"\n\n");
    message("system", "World imploding.\n", users());
    users()->quit_account();
}

private string trace_line (object obj, string prog, string file, int line) {
    string objfn = obj ? file_name(obj) : "<none>";
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

// Handles caught and runtime errors for the driver.
void error_handler (mapping e, int caught) {
    string ret, file = caught ? "catch" : "runtime";

    if (caught && sizeof(e["trace"]) > 1 && e["trace"][1]["program"] == D_TEST) {
        // debug_message("--- CAUGHT DURING TEST:\n" + standard_trace(e) + "\n---");
        return;
    }

    ret = "--- " + ctime(time()) + "\n" + standard_trace(e) + "\n";
    if (file_size("/log/"+file) > 20000) { // 20 kb
        rename("/log/"+file, "/log/"+file+"-"+time());
    }
    write_file("/log/"+file, ret);
    // @TODO
    // CHAT_D->do_chat("runtime", ret , 2 , 0);
    if (this_user(1) && !this_user()->query_property("updating")) {
        tell_object(this_user(1), sprintf("%sTrace written to /log/%s\n", e["error"], (caught ? "catch" : "runtime")));
        // this_user(1)->set_error(e);
    }
    return 0;
}

/*
    Whenever  an error occurs during compilation, the function log_error in
    the master object is called with the filename that the  error  occurred
    in  and  the error message itself.  Then, log_error is free to do what‐
    ever it thinks it should do with that  information.   Usually  this  is
    deciding  based  on  the  filename  where  the  error message should be
    logged, and then writing it to that file.

    void log_error(string file, string message);
*/
private string *read_file_disabled_warnings (string file) {
    string *lines = file_size(file) > 0 ? explode(read_file(file), "\n") : ({});
    lines = filter_array(lines, (: regexp($1,"// disable warning:") :));
    return map_array(lines, (: $1[strsrch($1, ": ")+2..<1] :));
}
void log_error (string file, string msg) {
    string dest, lcMsg, nom, tmp;

    if (base_name(previous_object()) == "/secure/daemon/test") debug_message(msg);

    if (file[0] != '/') file = "/" + file;
    if (sscanf(file, "/realm/%s/%s", nom, tmp) == 2) dest = nom;
    else if (sscanf(file, "/domain/%s/%s", nom, tmp) == 2) dest = nom;
    else if (sscanf(file, "/%s/%s", nom, tmp) == 2) dest = nom;
    if(!dest) dest = "log";

    if(regexp(msg, "Warning: ")) {
        lcMsg = lower_case(msg);
        foreach (string warning in read_file_disabled_warnings(file)) {
            if (regexp(lcMsg, lower_case(warning))) return;
        }
        msg = replace_string(msg, "Warning: ", "%^ORANGE%^Warning%^RESET%^: ", 1);
    } else {
        if (file_size("/log/" + dest) > 20000) { // 20 kb
            rename("/log/" + dest, "/log/" + dest+"-"+time());
        }
        write_file("/log/" + dest, ctime(time())+" "+msg);
        msg = replace_string(msg, ": ", ": %^RED%^BOLD%^Error%^RESET%^: ", 1);
    }
    if (msg && this_user(1) ) { // @TODO && (find_object(OB_SIMUL_EFUN) && creatorp(this_user(1)))) {
        message("error", msg, this_user(1));
    }
}

// --- ed applies --------------------------------------------------------------

// get_save_file_name
/*
    This  master apply is called by ed() when a player disconnects while in
    the editor and editing a file.  This function should return  an  alter‐
    nate file name for the file to be saved, to avoid overwriting the orig‐
    inal.

    string get_save_file_name(string filename?);
*/

// make_path_absolute
/*
    This  master apply is called by the ed() efun to resolves relative path
    names of a file to read/write, to an absolute path name.

    string make_path_absolute(string rel_path);
*/
string make_path_absolute (string rel_path) {
    return sanitize_path(rel_path);
}

// retrieve_ed_setup
/*
    This master apply is called by the ed() efun to retrieve  a  user's  ed
    setup/configuration  settings.   This  function should return the setup
    (contained in an int).

    int retrieve_ed_setup(object user);
*/

// save_ed_setup
/*
    This  master  apply is called by the ed() efun to save a user's ed set‐
    up/configuration settings (contained in an int).  This function  should
    return an int for success (1 or TRUE)/failure (0 or FALSE).

    int save_ed_setup(object user, int config);
*/

/*
Dead Souls:
int save_ed_setup(object who, int code) {
    string file;

    if(!intp(code)) return 0;
    rm(file = user_path(who->GetKeyName())+".edrc");
    return write_file(file, code+"");
}

int retrieve_ed_setup(object who) {
    string file;

    file = user_path(who->GetKeyName())+".edrc";
    if(!file_exists(file)) return 0;
    return to_int(read_file(file));
}

Lima:

private int save_ed_setup(object who, int code) {
  who->set_ed_setup(code);
  return 1;
}

private int retrieve_ed_setup(object who) {
  return who->query_ed_setup();
}
*/

// --- valid applies -----------------------------------------------------------

// valid_bind

// valid_database

// valid_hide

// valid_link

// valid_object

// Controls the use of efun:: prefix.
varargs int valid_override (string file, string efun_name, string main_file) {
    if (file[0] != '/') return 0;

    // debug_message("valid_override "+file+" "+efun_name+" "+main_file);

    switch (efun_name) {
        case "input_to":
        case "get_char":
            return file == "/secure/user/input";
        case "clone_object":
            return file == "/secure/module/test";
    }
    return regexp(file, "/secure/sefun");
}


// valid_save_binary

// valid_seteuid

// valid_shadow

// valid_socket


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
int valid_read(string file, mixed caller, string fn) {
    int valid = 0;
    file = sanitize_path(file);
    if (regexp(file_name(caller), "/secure/daemon/master") > 0) valid = 1; // @TODO this_object()
    // else if (regexp(file_name(caller), "/secure/daemon/test") > 0) valid = 1; // @TODO
    else if (regexp(file_name(caller), "/secure/daemon/access") > 0) valid = 1;
    else {
        // debug_message("master()->valid_read " + file_name(caller) + " wants " + fn + " on " + file);
        valid = D_ACCESS->query_allowed(caller, fn, file, "read");
        // if (!valid) log_file("access", "Read access denied: "+file+" : "+identify(previous_object(-1))+" : "+fn+"\n");
    }
    if (!valid) debug_message("master()->valid_read " + file + " " + file_name(caller) + " gets " + valid);
    return valid;
}
int valid_write(string file, mixed caller, string fn) {
    int valid = 0;
    file = sanitize_path(file);
    if (regexp(file_name(caller), "/secure/daemon/master") > 0) valid = 1; // @TODO this_object()
    else if (regexp(file_name(caller), "/secure/daemon/access") > 0) valid = 1;
    else {
        valid = D_ACCESS->query_allowed(caller, fn, file, "write");
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
    return ({ "in", "from", "on", "under", "behind", "beside", "of", "for","to",
      "with", "at", "off", "out", "down", "up", "around", "over", "into","onto",
      "about", "out of", "aboard", "above", "against", "alongside", "beneath",
      "below", "besides", "by", "inside", "outside", "through", "underneath",
      "upon", "within", "without", "alongside of", "back of", "down from",
      "inside of", "round about", "near to", "next to", "over to", "outside of",
      "up to", "in front of", "in back of", "on top of", "off of" });
}
// A word that refers to everything in an environment
string parse_command_all_word () {
    return "all";
}
/*
    This should return a list of living objects that can be refered to by
    commands that match remote living objects. Normally the objects
    examined when parsing a string are obtained from the deep_inventory()
    of the environment of the object that called parse_sentence() however
    in some cases you want commands to be able to match players who are not
    in the same room. This apply should return valid "remote living" objects.
    The response to this call is cached, and if it should change (e.g.
    someone logs in or out) then parse_refresh() must be called.
*/
object *parse_command_users () {
    return users()->query_character(); // @TODO characters() efun?
}
/*
    This apply is called by the parser to generate intelligent error
    messages in cases where rules have been "nearly matched". The
    parameters passed are the error code (defined in an parser_errors.h file
    packaged with fluffos), the object concerned (if known), data about the
    error (dependent on the error code) and whether or not the error was a
    "plural" error or not (i.e. the error data represents more than one
    object).
*/
// string parser_error_message (int error, object ob, mixed arg, int plural) {
//     // @TODO
//     return 0;
// }

string parser_error_message (int type, object ob, mixed arg, int plural) {
    string err;
    object tmpob;
    string wut;
    object wat;

    if (ob) err = ob->query_short();
    else err = "";

    debug_message("parser_error_message: "+type+" "+identify(ob)+" "+arg+" "+plural);
    switch (type) {
    case 0:
        if (arg && objectp(arg)) wat = arg;
        if (!wat && arg && arrayp(arg) && sizeof(arg)) {
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
        if (plural || (arg && stringp(arg)) && environment(this_character())) {
            // ???
            if (tmpob = present(arg, environment(this_character()))) {
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

/*
    This function is called whenever characters enter or exit the world.
*/
void handle_parse_refresh () {
    parse_refresh();
}