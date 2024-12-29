private mapping __Properties = ([ ]);

mapping query_properties () {
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }

    return __Properties;
}
mixed query_property (string key) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->query_property");
    }
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }

    return __Properties[key];
}

mixed set_property (string key, mixed value) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->set_property");
    }
    if (undefinedp(value)) {
        error("Bad argument 2 to property->set_property");
    }
    if (arrayp(value) || mapp(value)) {
        error("Bad argument 2 to property->set_property");
    }
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }

    return __Properties[key] = value;
}
mixed add_property (string key, mixed value) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->add_property");
    }
    if (undefinedp(value) || value == "") {
        error("Bad argument 2 to property->add_property");
    }
    if (arrayp(value) || mapp(value)) {
        error("Bad argument 2 to property->add_property");
    }
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }

    if (__Properties[key]) {
        __Properties[key] += value;
    } else {
        __Properties[key] = value;
    }
    return __Properties[key];
}
mapping set_properties (mapping properties) {
    if (!mapp(properties) || !sizeof(properties)) {
        error("Bad argument 1 to property->set_properties");
    }
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }

    if (sizeof(__Properties)) {
        __Properties += copy(properties);
    } else {
        __Properties = copy(properties);
    }
    return __Properties;
}

int remove_property (string key) {
    if (!stringp(key) || key == "") {
        error("Bad argument 1 to property->remove_property");
    }
    if (!mapp(__Properties)) {
        __Properties = ([ ]);
    }

    if (!undefinedp(__Properties[key])) {
        map_delete(__Properties, key);
    }
    return !__Properties[key];
}
mapping remove_properties () {
    return __Properties = ([ ]);
}