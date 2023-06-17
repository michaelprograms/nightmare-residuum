#define VALID_ATTRIBUTES ({ "build", "complexion", "eye", "hair", "height", })

private mapping __Attribute = ([ ]);

string *query_attributes () {
    if (!mapp(__Attribute)) {
        __Attribute = ([ ]);
    }
    return keys(__Attribute);
}

string query_attribute (string key) {
    if (!mapp(__Attribute)) {
        __Attribute = ([ ]);
    }
    return __Attribute[key];
}

void set_attribute (string key, string value) {
    if (member_array(key, VALID_ATTRIBUTES) == -1) {
        return;
    }
    if (!mapp(__Attribute)) {
        __Attribute = ([ ]);
    }
    __Attribute[key] = value;
}

string describe_attributes () {
    string *values = ({ });

    if (!mapp(__Attribute)) {
        __Attribute = ([ ]);
    }

    if (__Attribute["build"]) {
        values += ({ __Attribute["build"] + " build" });
    }
    if (__Attribute["complexion"]) {
        values += ({ __Attribute["complexion"] + " complexion" });
    }
    if (__Attribute["eye"]) {
        values += ({ __Attribute["eye"] + " eyes" });
    }
    if (__Attribute["hair"]) {
        values += ({ __Attribute["hair"] + " hair" });
    }
    if (__Attribute["height"]) {
        values += ({ __Attribute["height"] + " tall" });
    }

    return sizeof(values) ? conjunction(values) : "";
}