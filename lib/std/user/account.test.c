inherit M_TEST;

#define ACCOUNT_PATH "/save/account/a/accounttest.o"

private nosave object testOb;
void before_each_test () {
    testOb = clone_object("/std/user/account.c");
}
void after_each_test () {
    if (objectp(testOb)) destruct(testOb);
    if (unguarded((: file_size(ACCOUNT_PATH) :)) > -1) {
        unguarded((: rm(ACCOUNT_PATH) :));
    }
}

void test_account_name () {
    expect_function("query_name", testOb);
    expect_function("query_key_name", testOb);
    expect_function("set_name", testOb);

    expect("account name is settable and queryable", (: ({
        assert(testOb->query_name(), "==", UNDEFINED),
        testOb->set_name("accounttest"),
        assert(testOb->query_name(), "==", "accounttest"),
        assert(testOb->query_key_name(), "==", "accounttest"),
        assert(testOb->query_save_path(), "==", ACCOUNT_PATH),
        testOb->set_name("AccountTest"),
        assert(testOb->query_name(), "==", "AccountTest"),
        assert(testOb->query_key_name(), "==", "accounttest"),
        assert(testOb->query_save_path(), "==", ACCOUNT_PATH),
    }) :));
}

void test_account_password () {
    expect_function("set_password", testOb);
    expect_function("query_password", testOb);

    expect("account password is not settable", (: ({
        assert((: testOb->set_password("test") :), "catch", "*Illegal attempt to account->set_password\n"),
    }) :));
    expect("account test password is blank", (: ({
        assert(testOb->query_password(), "==", 0),
    }) :));
}

nosave private int now = time();

void test_account_times () {
    expect_function("query_created", testOb);
    expect_function("query_last_on", testOb);
    expect_function("set_last_on", testOb);

    expect("account times are valid", (: ({
        testOb->set_name("accounttest"),
        assert(testOb->query_save_path(), "==", ACCOUNT_PATH),
        testOb->save_data(),
        assert(unguarded((: file_size(ACCOUNT_PATH) :)) > 0, "==", 1),
        assert(testOb->query_created(), "==", now),
        assert(testOb->query_last_on(), "==", now),

        // read in accounttest.o and modify the __LastOn
        unguarded((: write_file(ACCOUNT_PATH, implode(map(explode(read_file(ACCOUNT_PATH), "\n"), function (string line) {
            if (regexp(line, "^__LastOn ")) {
                return "__LastOn " + (now - 100);
            }
            return line;
        }), "\n"), 1) :)),
        assert(unguarded((: file_size(ACCOUNT_PATH) :)) > 0, "==", 1),
        testOb->restore_data(),
        assert(testOb->query_last_on(), "==", now - 100),

        testOb->set_last_on(),
        assert(testOb->query_last_on(), "==", now),
    }) :));
}

void test_account_settings () {
    expect_function("query_setting", testOb);
    expect_function("query_settings", testOb);
    expect_function("set_setting", testOb);

    expect("account settings handles setting and querying", (: ({
        assert(mapp(testOb->query_settings()), "==", 1),
        assert(sizeof(testOb->query_settings()), "==", 5),

        assert(testOb->query_setting("ansi"), "==", "on"),
        testOb->set_setting("ansi", "off"),
        assert(testOb->query_setting("ansi"), "==", "off"),

        assert(testOb->query_setting("width"), "==", 80),
        testOb->set_setting("width", 60),
        assert(testOb->query_setting("width"), "==", 60),

        assert(testOb->query_setting("lines"), "==", 40),
        testOb->set_setting("lines", 50),
        assert(testOb->query_setting("lines"), "==", 50),
    }) :));
}

nosave private mapping tmpCharacter;
nosave private string __KeyName = "accounttest";
string query_key_name () { return __KeyName; }
string query_species () { return "human"; }
int query_level () { return 123; }
int query_last_action () { return 123456; }
nosave private string __LastEnvShort;
string query_environment_short () { return __LastEnvShort; }

void test_account_characters () {
    expect_function("query_character_names", testOb);
    expect_function("query_playable_characters", testOb);
    expect_function("query_character_by_name", testOb);
    expect_function("add_character", testOb);
    expect_function("update_character_data", testOb);
    expect_function("set_deleted", testOb);

    __KeyName = "tester";
    __LastEnvShort = "somewhere";

    expect("account characters can be added and deleted", (: ({
        assert(testOb->query_character_names(), "==", ({ })),
        assert(testOb->query_playable_characters(), "==", 0),

        testOb->add_character("Tester", "tester", "custodian"),

        assert(testOb->query_playable_characters(), "==", 1),
        assert(testOb->query_character_names(), "==", ({ "tester" })),
        assert(testOb->query_character_by_name("tester")["name"], "==", "Tester"),
        assert(testOb->query_character_by_name("tester")["deleted"], "==", 0),
        assert(testOb->query_character_by_name("tester")["species"], "==", "custodian"),
        assert(testOb->query_character_by_name("tester")["last_action"], "==", 0),
        assert(testOb->query_character_by_name("tester")["last_location"], "==", 0),
        assert(testOb->query_character_by_name("tester")["level"], "==", 0),

        // ensure character mapping data is a copy and not by reference
        assert(mapp(tmpCharacter = testOb->query_character_by_name("tester")), "==", 1),
        assert(tmpCharacter["deleted"] = 1, "==", 1),
        assert(testOb->query_character_by_name("tester")["deleted"], "==", 0),

        testOb->update_character_data(this_object()), // mock data functions above
        assert(testOb->query_character_by_name("tester")["deleted"], "==", 0),
        assert(testOb->query_character_by_name("tester")["species"], "==", "human"),
        assert(testOb->query_character_by_name("tester")["last_action"], "==", 123456),
        assert(testOb->query_character_by_name("tester")["last_location"], "==", "somewhere"),
        assert(testOb->query_character_by_name("tester")["level"], "==", 123),

        testOb->set_deleted("tester"),
        assert(testOb->query_character_by_name("tester")["deleted"], "==", 1),
        assert(testOb->query_character_names(), "==", ({ })),
        assert(testOb->query_playable_characters(), "==", 0),
    }) :));

    __KeyName = "accounttest";
    __LastEnvShort = 0;
}

void test_ensure_default_settings () {
    mapping settings;
    expect_function("ensure_default_settings", testOb);

    settings = testOb->query_settings();
    expect("ensure_default_settings sets missing settings", (: ({
        // initial
        assert(sizeof(testOb->query_settings()), "==", 5),
        // remove width
        map_delete($(settings), "width"),
        assert(testOb->query_setting("width"), "==", 0),
        // remove lines
        map_delete($(settings), "lines"),
        assert(testOb->query_setting("lines"), "==", 0),
        // remove ansi
        map_delete($(settings), "ansi"),
        assert(testOb->query_setting("ansi"), "==", 0),
        // remove screenreader
        map_delete($(settings), "screenreader"),
        assert(testOb->query_setting("screenreader"), "==", 0),
        // remove gmcp
        map_delete($(settings), "gmcp"),
        assert(testOb->query_setting("gmcp"), "==", 0),
        // now empty
        assert(sizeof(testOb->query_settings()), "==", 0),
        // restore defaults
        testOb->ensure_default_settings(),
        assert(sizeof(testOb->query_settings()), "==", 5),
        assert(testOb->query_setting("width"), "==", 80),
        assert(testOb->query_setting("lines"), "==", 40),
        assert(testOb->query_setting("ansi"), "==", "on"),
        assert(testOb->query_setting("screenreader"), "==", "off"),
    }) :));
}

void test_ed_setup () {
    expect_function("query_ed_setup", testOb);
    expect_function("set_ed_setup", testOb);

    expect("ed setup is settable and queryable", (: ({
        assert(testOb->query_ed_setup(), "==", UNDEFINED),

        testOb->set_ed_setup(123),
        assert(testOb->query_ed_setup(), "==", 123),

        testOb->set_ed_setup(321),
        assert(testOb->query_ed_setup(), "==", 321),
    }) :));
}