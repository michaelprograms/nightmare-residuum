inherit M_PARENT;
inherit M_SAVE;
inherit M_PROPERTY;

private string __Name;
private string __Password;
private int __Created = time();
private int __LastOn = time();
private mapping __Characters = ([
    /* Data Format:
        STRING: ([                      // Name sanitized version
            "species":          STRING, // Species type for BODY
            "deleted":          INT,    // 0 or 1
            "level":            INT,
            "name":             STRING, // Name formatted version
            "last_action":      INT,    // last seen
            "last_location":    STRING  // last location short
        ]),
    */
]);
private mapping __Settings = ([ ]);
private int __EdSetup;  // @TODO


// cache variable, @TODO should probably be renamed
nosave private string *__CharacterNames = ({});

// -----------------------------------------------------------------------------

void ensure_default_settings () {
    // verify default settings exist
    if (undefinedp(__Settings["width"])) {
        __Settings["width"] = 80;
    }
    if (undefinedp(__Settings["lines"])) {
        __Settings["lines"] = 40;
    }
    if (undefinedp(__Settings["ansi"])) {
        __Settings["ansi"] = "on";
    }
    if (undefinedp(__Settings["screenreader"])) {
        __Settings["screenreader"] = "off";
    }
}

void create () {
    parent::create();
    ensure_default_settings();
}

int is_account () { return 1; }

void set_name (string name) { // @TODO setup_account?
    __Name = name;
    if (!stringp(__Name)) {  // name can be cleared during account creation
        return;
    }
    set_save_path(D_ACCOUNT->query_save_path(lower_case(__Name)));
    if (D_ACCOUNT->query_exists(__Name)) {
        restore_data();
    }

    ensure_default_settings();
}

string query_name () {
    return __Name;
}
string query_key_name () {
    return __Name ? lower_case(__Name) : 0;
}

nomask void set_password (string str) {
    if (base_name(previous_object()) != "/secure/user/user") {  // TODO better way to match?
        error("Illegal attempt to account->set_password");
    }
    __Password = str;
    save_data();
}
nomask string query_password () {
    return __Password;
}

int query_created () {
    return __Created;
}
int query_last_on () {
    return __LastOn;
}
void set_last_on () {
    __LastOn = time();
}

void add_character (string name, string nameClean, string species) {
    // @TODO security?
    __Characters[nameClean] = ([
        "name": name,
        "deleted": 0,
        "species": species,
        "last_action": 0,
        "last_location": 0,
        "level": 0,
    ]);
    save_data();
}
mapping query_character (string name) {
    return copy(__Characters[name]);
}
int query_playable_characters () {
    mapping tmpCharacters = filter_mapping(__Characters, (: !$2["deleted"] :));
    __CharacterNames = sort_array(keys(tmpCharacters), 1);
    return sizeof(__CharacterNames) > 0;
}
string *query_character_names () {
    mapping tmpCharacters = filter_mapping(__Characters, (: !$2["deleted"] :));
    __CharacterNames = sort_array(keys(tmpCharacters), 1);
    return __CharacterNames;
}
void update_character_data (object character) {
    string name;
    if (!character || !(name = character->query_key_name())) {
        return;
    }
    __Characters[name]["species"] = character->query_species();
    __Characters[name]["level"] = character->query_level();
    __Characters[name]["last_action"] = character->query_last_action();
    __Characters[name]["last_location"] = character->query_environment_short();
    save_data();
}

void set_deleted (string name) {
    __Characters[name]["deleted"] = 1;
}

void set_setting (string key, mixed value) {
    if (!__Settings || !key) return;
    __Settings[key] = value;
    save_data();
}
mixed query_setting (string key) {
    if (!__Settings || !key) return 0;
    if (member_array(key, keys(__Settings)) == -1) return 0;
    return __Settings[key];
}
mapping query_settings () {
    return __Settings;
}
