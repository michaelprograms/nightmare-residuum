inherit M_TEST;

#define PATH_accounttest "/save/account/a/accounttest.o"
#define PATH_AccountTest "/save/account/A/AccountTest.o"

/**
 * @var {"/std/user/account"} testOb
 */

void after_each_test () {
    if (file_size(PATH_accounttest) > -1) {
        rm(PATH_accounttest);
    }
    if (file_size(PATH_AccountTest) > -1) {
        rm(PATH_AccountTest);
    }
}

void test_account_name () {
    expect("account name is settable and queryable", (: ({
        assert_equal(testOb->query_name(), UNDEFINED),
        testOb->set_name("accounttest"),
        assert_equal(testOb->query_name(), "accounttest"),
        assert_equal(testOb->query_key_name(), "accounttest"),
        assert_equal(testOb->query_save_path(), PATH_accounttest),
        testOb->set_name("AccountTest"),
        assert_equal(testOb->query_name(), "AccountTest"),
        assert_equal(testOb->query_key_name(), "AccountTest"),
        assert_equal(testOb->query_save_path(), PATH_AccountTest),
    }) :));
}

void test_account_password () {
    expect("account password is not settable", (: ({
        assert_catch((: testOb->set_password("test") :), "*Illegal attempt to account->set_password\n"),
    }) :));
    expect("account test password is blank", (: ({
        assert_equal(testOb->query_password(), 0),
    }) :));
}

nosave private int now;

void test_account_times () {
    expect("account times are valid", (: ({
        // verify current time exists
        assert_equal((now = time()) > 0, 1),

        testOb->set_name("accounttest"),
        assert_equal(testOb->query_save_path(), PATH_accounttest),
        testOb->save_data(),
        assert_equal(file_size(PATH_accounttest) > 0, 1),
        assert_equal(testOb->query_created(), now),
        assert_equal(testOb->query_last_on(), now),

        // read in accounttest.o and modify the __LastOn
        write_file(PATH_accounttest, implode(map(explode(read_file(PATH_accounttest), "\n"), function (string line) {
            if (regexp(line, "^__LastOn ")) {
                return "__LastOn " + (now - 100);
            }
            return line;
        }), "\n"), 1),
        assert_equal(file_size(PATH_accounttest) > 0, 1),
        testOb->restore_data(),
        assert_equal(testOb->query_last_on(), now - 100),

        testOb->set_last_on(),
        assert_equal(testOb->query_last_on(), now),
    }) :));
}

void test_account_settings () {
    expect("account settings handles setting and querying", (: ({
        assert_equal(mapp(testOb->query_settings()), 1),
        assert_equal(sizeof(testOb->query_settings()), 6),

        assert_equal(testOb->query_setting("ansi"), "on"),
        testOb->set_setting("ansi", "off"),
        assert_equal(testOb->query_setting("ansi"), "off"),

        assert_equal(testOb->query_setting("width"), 80),
        testOb->set_setting("width", 60),
        assert_equal(testOb->query_setting("width"), 60),

        assert_equal(testOb->query_setting("lines"), 40),
        testOb->set_setting("lines", 50),
        assert_equal(testOb->query_setting("lines"), 50),
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
    __KeyName = "tester";
    __LastEnvShort = "somewhere";

    expect("account characters can be added and deleted", (: ({
        assert_equal(testOb->query_character_names(), ({ })),
        assert_equal(testOb->query_has_playable_characters(), 0),

        testOb->add_character("Tester", "tester", "custodian"),

        assert_equal(testOb->query_has_playable_characters(), 1),
        assert_equal(testOb->query_character_names(), ({ "tester" })),
        assert_equal(testOb->query_character_by_name("tester")["name"], "Tester"),
        assert_equal(testOb->query_character_by_name("tester")["deleted"], 0),
        assert_equal(testOb->query_character_by_name("tester")["species"], "custodian"),
        assert_equal(testOb->query_character_by_name("tester")["last_action"], 0),
        assert_equal(testOb->query_character_by_name("tester")["last_location"], 0),
        assert_equal(testOb->query_character_by_name("tester")["level"], 0),

        // ensure character mapping data is a copy and not by reference
        assert_equal(mapp(tmpCharacter = testOb->query_character_by_name("tester")), 1),
        assert_equal(tmpCharacter["deleted"] = 1, 1),
        assert_equal(testOb->query_character_by_name("tester")["deleted"], 0),

        // nothing to update
        testOb->update_character_data(UNDEFINED), // mock data functions above

        // updates character
        testOb->update_character_data(this_object()), // mock data functions above
        assert_equal(testOb->query_character_by_name("tester")["deleted"], 0),
        assert_equal(testOb->query_character_by_name("tester")["species"], "human"),
        assert_equal(testOb->query_character_by_name("tester")["last_action"], 123456),
        assert_equal(testOb->query_character_by_name("tester")["last_location"], "somewhere"),
        assert_equal(testOb->query_character_by_name("tester")["level"], 123),

        testOb->set_deleted("tester"),
        assert_equal(testOb->query_character_by_name("tester")["deleted"], 1),
        assert_equal(testOb->query_character_names(), ({ })),
        assert_equal(testOb->query_has_playable_characters(), 0),
    }) :));

    __KeyName = "accounttest";
    __LastEnvShort = 0;
}

void test_ensure_default_settings () {
    mapping settings;

    settings = testOb->query_settings();
    expect("ensure_default_settings sets missing settings", (: ({
        // initial
        assert_equal(sizeof(testOb->query_settings()), 6),
        // remove width
        map_delete($(settings), "width"),
        assert_equal(testOb->query_setting("width"), 0),
        // remove lines
        map_delete($(settings), "lines"),
        assert_equal(testOb->query_setting("lines"), 0),
        // remove ansi
        map_delete($(settings), "ansi"),
        assert_equal(testOb->query_setting("ansi"), 0),
        // remove screenreader
        map_delete($(settings), "screenreader"),
        assert_equal(testOb->query_setting("screenreader"), 0),
        // remove gmcp
        map_delete($(settings), "gmcp"),
        assert_equal(testOb->query_setting("gmcp"), 0),
        // remove autojoin
        map_delete($(settings), "autojoin"),
        assert_equal(testOb->query_setting("autojoin"), 0),
        // now empty
        assert_equal(sizeof(testOb->query_settings()), 0),
        // restore defaults
        testOb->ensure_default_settings(),
        assert_equal(sizeof(testOb->query_settings()), 6),
        assert_equal(testOb->query_setting("width"), 80),
        assert_equal(testOb->query_setting("lines"), 40),
        assert_equal(testOb->query_setting("ansi"), "on"),
        assert_equal(testOb->query_setting("screenreader"), "off"),
    }) :));
}

void test_ed_setup () {
    expect("ed setup is settable and queryable", (: ({
        assert_equal(testOb->query_ed_setup(), UNDEFINED),

        testOb->set_ed_setup(123),
        assert_equal(testOb->query_ed_setup(), 123),

        testOb->set_ed_setup(321),
        assert_equal(testOb->query_ed_setup(), 321),
    }) :));
}