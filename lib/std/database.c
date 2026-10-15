inherit M_CLEAN;

nosave string dbHost = "";
nosave string dbDatabase;
nosave string dbUser = "";
nosave int dbType = __USE_SQLITE3__;
nosave mixed dbHandle;

#define UNDEFINED ([ ])[0]

mixed query_handle() {
    return dbHandle;
}

/* -----  ----- */

mixed connect(mapping config) {
    mixed handle;

    if (!mapp(config)) {
        error("Bad argument 1 to database->connect");
    }
    if (dbHandle) {
        error("Database already connected");
    }

    if (stringp(config["host"])) {
        dbHost = config["host"];
    }
    if (undefinedp(config["db"])) {
        error("Bad argument 1 to database->connect: missing db");
    }
    dbDatabase = config["db"];
    if (config["user"]) {
        dbUser = config["user"];
    }
    if (!undefinedp(config["type"])) {
        dbType = config["type"];
    }

    // keep a failed connect's error string out of dbHandle
    handle = db_connect(dbHost, dbDatabase, dbUser, dbType);
    if (stringp(handle)) {
        error(handle);
    }
    dbHandle = handle;
    return this_object();
}

mixed close() {
    mixed result;
    if (dbHandle || (dbHandle == 0 && dbHandle != UNDEFINED)) {
        result = db_close(dbHandle);
        dbHandle = 0;
    }
    return result;
}

mixed query(string sql) {
    mixed response, *result;

    if (!dbHandle) {
        error("Database not connected");
    }

    response = db_exec(dbHandle, sql);
    if (stringp(response)) {
        close();
        error(response);
    } else if (intp(response) && response > 0) {
        result = allocate(response);
        for (int i = 0; i < response; i++) {
            result[i] = db_fetch(dbHandle, i + 1);
        }
    }
    return result;
}

/* -----  ----- */

int handle_remove() {
    close();
    return ::handle_remove();
}
