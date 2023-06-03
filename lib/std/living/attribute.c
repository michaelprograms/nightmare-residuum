#define VALID_ATTRIBUTES ({ "body", "eye", "hair", "height", "skin", })

private mapping __Attribute = ([ ]);

string *query_attributes () {
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