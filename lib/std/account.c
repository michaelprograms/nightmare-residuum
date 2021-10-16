inherit M_SAVE;

private string __AccountName;
private string __AccountPassword;
private int __AccountCreated = time();
private int __AccountLastOn = time();
private mapping __AccountCharacters = ([
    /* Data Format:
        STRING: ([                      // Name sanitized version
            "type":             STRING, // Body type for BODY
            "deleted":          INT,    // 0 or 1
            "level":            INT,
            "name":             STRING, // Name formatted version
            "last_action":      INT,    // last seen
            "last_location":    STRING  // last location short
        ]),
    */
]);
private mapping __AccountSettings = ([
    // default settings
    "width": 80,
    "ansi": "on",
    "screenreader": "off",
]);
// private mapping __AcountProperties = ([]); // @TODO
private int __EdSetup;  // @TODO

// cache variable, @TODO should probably be renamed
nosave private string *__AccountCharacterNames = ({});

// -----------------------------------------------------------------------------

int is_account () { return 1; }

void set_name (string name) {
    __AccountName = name;
    if (!stringp(__AccountName)) {  // name can be cleared during account creation
        return;
    }
    set_save_path(D_ACCOUNT->query_save_path(lower_case(__AccountName)));
    if (D_ACCOUNT->query_exists(__AccountName)) {
        restore_data();
    }
}
string query_name () {
    return __AccountName;
}
string query_key_name () {
    return __AccountName ? lower_case(__AccountName) : 0;
}

nomask void set_password (string str) {
    if (base_name(previous_object()) != "/secure/user/user") {  // TODO better way to match?
        error("illegal attempt to set_password");
    }
    __AccountPassword = str;
    save_data();
}
nomask string query_password () {
    return __AccountPassword;
}

int query_created () {
    return __AccountCreated;
}
int query_last_on () {
    return __AccountLastOn;
}
void set_last_on () {
    __AccountLastOn = time();
    save_data();
}

void add_character (string name, string nameClean, string type) {
    // @TODO security?
    __AccountCharacters[nameClean] = ([
        "name": name,
        "deleted": 0,
        "type": type,
        "last_action": 0,
        "last_location": 0,
        "level": 0,
    ]);
    save_data();
}
mapping query_character (string name) {
    return __AccountCharacters[name];
}
int query_has_playable_characters () {
    mapping tmpCharacters = filter_mapping(__AccountCharacters, (: !$2["deleted"] :));
    __AccountCharacterNames = sort_array(keys(tmpCharacters), 1);
    return sizeof(__AccountCharacterNames) > 0;
}
string *query_character_names () {
    return __AccountCharacterNames;
}
void update_character_data (object character) {
    string name;
    if (!character || !(name = character->query_key_name())) {
        return;
    }
    __AccountCharacters[name]["type"] = character->query_type();
    __AccountCharacters[name]["level"] = character->query_level();
    __AccountCharacters[name]["last_action"] = character->query_last_action();
    __AccountCharacters[name]["last_location"] = character->query_last_location();
    save_data();
}

void set_deleted (string name) {
    __AccountCharacters[name]["deleted"] = 1;
    save_data();
}

void set_setting (string key, mixed value) {
    if (!__AccountSettings || !key) return;
    __AccountSettings[key] = value;
    save_data();
}
mixed query_setting (string key) {
    if (!__AccountSettings || !key) return 0;
    if (member_array(key, keys(__AccountSettings)) == -1) return 0;
    return __AccountSettings[key];
}
mapping query_settings () {
    return __AccountSettings;
}