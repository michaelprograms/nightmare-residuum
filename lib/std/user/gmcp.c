mapping __GMCPModuleData = ([ ]);

int gmcp_enabled() {
    if (this_object()->query_setting("gmcp") != "on") {
        return 0;
    }
    return has_gmcp(this_object());
}

void gmcp_send_update (string module, mapping data) {
    if (!gmcp_enabled()) {
        return;
    }

    if (!mapp(__GMCPModuleData)) {
        __GMCPModuleData = ([ ]);
    }
    if (!mapp(__GMCPModuleData[module])) {
        __GMCPModuleData[module] = ([ ]);
    }
    __GMCPModuleData[module] += data;

    send_gmcp(module + " " + json_encode(data));
}