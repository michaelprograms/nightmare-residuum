#define VALID_ATTRIBUTES ({ "build", "complexion", "eye", "hair", "height", })

mapping __Attribute = ([ ]);

private void initialize_attributes () {
    if (!mapp(__Attribute)) {
        __Attribute = ([ ]);
    }
}

mapping query_attributes () {
    initialize_attributes();
    return __Attribute;
}

string query_attribute (string key) {
    initialize_attributes();
    return __Attribute[key];
}

void set_attribute (string key, string value) {
    initialize_attributes();
    if (member_array(key, VALID_ATTRIBUTES) == -1) {
        return;
    }
    __Attribute[key] = value;
}