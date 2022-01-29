
#include "object.h"

nosave private string __Name;
nosave private string __KeyName;
nosave private string __KeyID;
nosave private string __KeyAdj;
nosave private string *__ID = ({});
nosave private string *__Adj = ({});
nosave private string *__Plural = ({});

// -----------------------------------------------------------------------------

string query_name () {
    return __Name;
}
string query_key_name () {
    return __KeyName;
}
void set_name (string name) {
    if (!stringp(name) || name == "") error("Bad argument 1 to id->set_name");
    __Name = name;
    __KeyName = sanitize_name(name);
    refresh_id();
}
void set_key_name (string name) {
    if (!stringp(name) || name == "") error("Bad argument 1 to id->set_key_name");
    __KeyName = lower_case(name);
    refresh_id();
}

string *query_id () {
    return __ID;
}
string query_key_id () {
    return __KeyID;
}
void set_id (string *ids) {
    if (undefinedp(ids) || !arrayp(ids)) error("Bad argument 1 to id->set_id");
    foreach (string id in ids) {
        if (member_array(id, __ID) == -1) {
            __ID += ({ id });
            __Plural += ({ pluralize(id) });
        }
    }
    refresh_id();
}
void remove_id (string *id) {
    if (undefinedp(id) || !arrayp(id)) error("Bad argument 1 to id->remove_id");
    __ID -= id;
    __Plural -= map_array(id, (: pluralize :));
    __KeyID = 0;
    refresh_id();
}

string *query_adjective () {
    return __Adj;
}
string query_key_adjective () {
    return __KeyAdj;
}
void set_adjective (string *adj) {
    if (undefinedp(adj) || !arrayp(adj)) error("Bad argument 1 to id->set_adjective");
    __Adj = filter_array(__Adj + adj, (: $1 && $1 != "" :));
    refresh_id();
}
void add_adjective (string adj) {
    if (!stringp(adj) || adj == "") error("Bad argument 1 to id->add_adjective");
    __Adj += ({ adj });
    refresh_id();
}
void remove_adjective (string adj) {
    if (!stringp(adj) || adj == "") error("Bad argument 1 to id->add_adjective");
    __Adj -= ({ adj });
    if (__KeyAdj == adj) {
        __KeyAdj = 0;
    }
    refresh_id();
}

string *query_plural () {
    return __Plural;
}
void add_plural (string plural) {
    if (!stringp(plural) || plural == "") error("Bad argument 1 to id->add_plural");
    __Plural += ({ plural });
    refresh_id();
}
void remove_plural (string plural) {
    if (!stringp(plural) || plural == "") error("Bad argument 1 to id->remove_plural");
    __Plural -= ({ plural });
    refresh_id();
}

// -----------------------------------------------------------------------------

private void refresh_id () {
    // write("\nid refresh "+identify(this_object())+"\n");
    // write("__Name: "+__Name+" __KeyName: "+__KeyName+"\n");
    // write("__KeyID: "+__KeyID+" __KeyAdj: "+__KeyAdj+"\n");
    // write("__ID: "+identify(__ID)+"\n");
    // write("__Adj: "+identify(__Adj)+"\n");
    // write("__Plural: "+identify(__Plural)+"\n");

    if (!__KeyID && sizeof(__ID)) {
        __KeyID = __ID[0];
    }
    if (!__KeyAdj && sizeof(__Adj)) {
        __KeyAdj = __Adj[0];
    }
    // if (__KeyID) {
    //     if (__KeyAdj) __Short = __KeyAdj + " " + __KeyID;
    //     else __Short = __KeyID;
    // } else __Short = "nondescript thing";

    if (__KeyName && member_array(__KeyName, __ID) == -1) {
        set_id(({ __KeyName }));
    }

    parse_refresh();
}

void create () {
    parse_init();
    __ID = ({ });
    __Adj = ({ });
    __Plural = ({ });
    refresh_id();
}

// --- parser applies ----------------------------------------------------------

// List of nouns that apply to this object
string *parse_command_id_list () {
    return __ID;
}
// List of adjectives that apply to this object
string *parse_command_adjectiv_id_list () {
    return __Adj;
}
// List of plurals that apply to this object
string *parse_command_plural_id_list () {
    return __Plural;
}