nosave private string __Name;
nosave private string __CapName;
nosave private string __KeyName;
nosave private string __KeyID;
nosave private string __KeyAdj;
nosave private string *__ID = ({});
nosave private string *__Adj = ({});
nosave private string *__Plural = ({});

// -----------------------------------------------------------------------------

string query_name ();
string query_cap_name ();
string query_key_name ();
void set_name (string name);
void set_key_name (string name);
string *query_id ();
string query_key_id ();
void set_id (string *id);
void remove_id (string *id);
string *query_adjective ();
void set_adjective (string *adjs);
void add_adjective (string adj);
void remove_adjective (string adj);
void refresh_id ();
string *parse_command_id_list ();
string *parse_command_adjectiv_id_list ();
string *parse_command_plural_id_list ();

// -----------------------------------------------------------------------------

string query_name () {
    return __Name;
}
string query_cap_name () {
    return __CapName;
}
string query_key_name () {
    return __KeyName;
}
void set_name (string name) {
    if (!stringp(name) || name == "") {
        error("Bad argument 1 to id->set_name");
    }
    __Name = lower_case(name);
    __CapName = capitalize(name);
    __KeyName = sanitize_name(name);
    refresh_id();
}
void set_key_name (string name) {
    if (!stringp(name) || name == "") {
        error("Bad argument 1 to id->set_key_name");
    }
    __KeyName = lower_case(name);
    refresh_id();
}

string *query_id () {
    return __ID;
}
string query_key_id () {
    return __KeyID;
}
void set_id (string *id) {
    if (undefinedp(id) || !arrayp(id) || !sizeof(id)) {
        error("Bad argument 1 to id->set_id");
    }
    foreach (string newID in id) {
        if (member_array(newID, __ID) == -1) {
            __ID += ({ newID });
            __Plural += ({ pluralize(newID) });
        }
    }
    refresh_id();
}
void remove_id (string *id) {
    if (undefinedp(id) || !arrayp(id) || !sizeof(id)) {
        error("Bad argument 1 to id->remove_id");
    }
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
    if (undefinedp(adj) || !arrayp(adj) || !sizeof(adj)) {
        error("Bad argument 1 to id->set_adjective");
    }
    __Adj = filter(__Adj + adj, (: $1 && $1 != "" :));
    refresh_id();
}
void add_adjective (string adj) {
    if (!stringp(adj) || adj == "") {
        error("Bad argument 1 to id->add_adjective");
    }
    __Adj += ({ adj });
    refresh_id();
}
void remove_adjective (string adj) {
    if (!stringp(adj) || adj == "") {
        error("Bad argument 1 to id->remove_adjective");
    }
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
    if (!stringp(plural) || plural == "") {
        error("Bad argument 1 to id->add_plural");
    }
    __Plural += ({ plural });
    refresh_id();
}
void remove_plural (string plural) {
    if (!stringp(plural) || plural == "") {
        error("Bad argument 1 to id->remove_plural");
    }
    __Plural -= ({ plural });
    refresh_id();
}

// -----------------------------------------------------------------------------

private void refresh_id () {
    if (!__KeyID && sizeof(__ID)) {
        __KeyID = __ID[0];
    }
    if (!__KeyAdj && sizeof(__Adj)) {
        __KeyAdj = __Adj[0];
    }
    // @TODO __Short?
    // if (__KeyID) {
    //     if (__KeyAdj) __Short = __KeyAdj + " " + __KeyID;
    //     else __Short = __KeyID;
    // } else __Short = "impossible to comprehend";

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