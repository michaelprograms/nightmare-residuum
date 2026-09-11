inherit M_TEST;

/**
 * @var {"/std/character"} testOb
 */

void test_npc() {
    expect("is_character behaves", (: ({
        assert_equal(livingp(testOb), 1),
        assert_equal(characterp(testOb), 1),
        assert_equal(npcp(testOb), 0),
    }) :));
}

void test_user() {
    expect("user is settable and queryable", (: ({
        assert_equal(testOb->query_user(), UNDEFINED),
        testOb->set_user(this_object()),
        assert_equal(testOb->query_user(), this_object()),
        testOb->set_user(0),
        assert_equal(testOb->query_user(), 0),
    }) :));
}

void test_account() {
    expect("account is settable and queryable", (: ({
        assert_equal(testOb->query_account(), UNDEFINED),
        testOb->set_account("test account"),
        assert_equal(testOb->query_account(), "test account"),
        testOb->set_account(0),
        assert_equal(testOb->query_account(), 0),
    }) :));
}

void test_times() {
    expect("user has a created time", (: ({
        // creation time is now
        assert_equal(testOb->query_created(), time()),
    }) :));
    expect("user has a last action time", (: ({
        // last action time is now
        assert_equal(testOb->query_last_action(), UNDEFINED),
        testOb->set_last_action(),
        assert_equal(testOb->query_last_action(), time()),
    }) :));
    expect("user has a connection time", (: ({
        // last action time is now
        assert_equal(testOb->query_connection_time(), 0),
    }) :));
}

void test_name() {
    expect("name handles character setup", (: ({
        assert_equal(testOb->query_name(), UNDEFINED),

        testOb->set_name("testcharacter"),
        assert_equal(testOb->query_name(), "testcharacter"),
        assert_equal(testOb->query_key_name(), "testcharacter"),
        assert_equal(testOb->query_cap_name(), "Testcharacter"),
        assert_equal(testOb->query_short(), "Testcharacter the character"),
        assert_equal(testOb->query_long(), "Testcharacter is an unknown."),
        assert_equal(
            testOb->query_save_path(),
            "/save/character/t/testcharacter/character.o"
        ),
    }) :));
}

void test_immortal() {
    expect("immortal is settable and queryable", (: ({
        assert_equal(testOb->query_immortal(), UNDEFINED),

        testOb->set_name("testcharacter"),
        assert_equal(testOb->query_name(), "testcharacter"),
        assert_equal(testOb->query_short(), "Testcharacter the character"),

        // verify turning off while off
        testOb->set_immortal(0),
        assert_equal(testOb->query_immortal(), 0),
        assert_equal(file_size("/realm/testcharacter"), -1),

        // verify turning on
        testOb->set_immortal(1),
        assert_equal(testOb->query_immortal(), 1),
        assert_equal(file_size("/realm/testcharacter"), -2),

        // verify turning off, realm dir remains
        testOb->set_immortal(0),
        assert_equal(testOb->query_immortal(), 0),
        assert_equal(file_size("/realm/testcharacter"), -2),

        assert_equal(rmdir("/realm/testcharacter"), 1),
    }) :));
}

nosave private string gmcpName;
nosave private mapping gmcpData;
void gmcp_send_update(string n, mapping m) {
    gmcpName = n;
    gmcpData = m;
}

// the test object doubles as the character's __User: these stubs stand in for
// the settings/message/save hooks the character calls back into.
nosave private string __ScreenReader = "off";
nosave private int __Width = 80;
nosave private string *__Received = ({});
mixed query_setting(string s) {
    if (s == "width") {
        return __Width;
    }
    if (s == "screenreader") {
        return __ScreenReader;
    }
    return 0;
}
void receive_message(string type, string msg) {
    __Received += ({ msg });
}
void update_character_data(object ob) {}
void test_gmcp() {
    testOb->set_level(1);
    testOb->set_species("human");

    expect("gmcp vitals request behaves", (: ({
        // no user, should fail
        testOb->gmcp_update_vitals(),
        assert_equal(gmcpName, UNDEFINED),
        assert_equal(gmcpData, UNDEFINED),

        // user, should succeed
        testOb->set_user(this_object()),
        testOb->gmcp_update_vitals(),
        assert_equal(gmcpName, "Char.Vitals"),
        assert_equal(
            gmcpData,
            ([
                "hp": 22,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 12,
                "sp": 12
            ])
        ),

        assert_equal(gmcpData = UNDEFINED, UNDEFINED),
        // vitals functions call gmcp
        testOb->set_hp(5),
        assert_equal(
            gmcpData,
            ([
                "hp": 5,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 12,
                "sp": 12
            ])
        ),
        testOb->add_hp(5),
        assert_equal(
            gmcpData,
            ([
                "hp": 10,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 12,
                "sp": 12
            ])
        ),

        testOb->set_sp(5),
        assert_equal(
            gmcpData,
            ([
                "hp": 10,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 12,
                "sp": 5
            ])
        ),
        testOb->add_sp(5),
        assert_equal(
            gmcpData,
            ([
                "hp": 10,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 12,
                "sp": 10
            ])
        ),

        testOb->set_mp(5),
        assert_equal(
            gmcpData,
            ([
                "hp": 10,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 5,
                "sp": 10
            ])
        ),
        testOb->add_mp(5),
        assert_equal(
            gmcpData,
            ([
                "hp": 10,
                "maxhp": 22,
                "maxmp": 12,
                "maxsp": 12,
                "mp": 10,
                "sp": 10
            ])
        ),
    }) :));
}

void test_go() {
    object r1 = new(STD_ROOM);
    object r2 = new(STD_ROOM);

    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    testOb->set_name("Test");
    testOb->set_user(this_object());

    expect("character can go", (: ({
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(testOb->handle_go(base_name($(r2)), "walk", "east",), 1),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}

void test_character_short() {
    testOb->set_name("Shorty");

    expect("query_character_short reflects immortal status", (: ({
        assert_equal(testOb->query_character_short(), "Shorty the character"),
        testOb->set_immortal(1),
        assert_equal(testOb->query_character_short(), "Shorty the immortal"),
        testOb->set_immortal(0),
        rmdir("/realm/shorty"),
    }) :));
}

void test_save_restore() {
    testOb->set_name("Saver");
    testOb->set_user(this_object());
    testOb->set_level(1);

    expect("save and restore round-trip", (: ({
        // exit == 0: keeps properties, notifies the user
        testOb->save_character(0),
        assert_equal(file_size("/save/character/s/saver/character.o") > 0, 1),
        // exit == 1: removes properties before saving
        testOb->save_character(1),
        assert_equal(file_size("/save/character/s/saver/character.o") > 0, 1),
        testOb->restore_character(),
        assert_equal(testOb->query_level(), 1),
    }) :));

    rm("/save/character/s/saver/character.o");
    rmdir("/save/character/s/saver");
    rmdir("/save/character/s");
}

void test_heart_beat() {
    int i;

    testOb->set_name("Beater");
    testOb->set_user(this_object());
    testOb->set_level(1);

    // 60 beats advances connection time and triggers one autosave (% 60 == 0)
    for (i = 0; i < 60; i++) {
        testOb->heart_beat();
    }

    expect("heart_beat advances connection time and autosaves", (: ({
        assert_equal(testOb->query_connection_time(), 60),
        assert_equal(file_size("/save/character/b/beater/character.o") > 0, 1),
    }) :));

    rm("/save/character/b/beater/character.o");
    rmdir("/save/character/b/beater");
    rmdir("/save/character/b");
}

void test_setup_character() {
    testOb->set_name("Setuper");
    testOb->set_user(this_object());
    testOb->set_species("human");

    expect("setup_character initializes a new character then restores", (: ({
        // no save exists: the new-character branch runs and saves
        testOb->setup_character(),
        assert_equal(testOb->query_level(), 1),
        assert_equal(file_size("/save/character/s/setuper/character.o") > 0, 1),
        // save exists now: the restore branch runs
        testOb->setup_character(),
        assert_equal(testOb->query_level(), 1),
    }) :));

    rm("/save/character/s/setuper/character.o");
    rmdir("/save/character/s/setuper");
    rmdir("/save/character/s");
}

void test_describe_environment() {
    object r = new(STD_ROOM);
    object r2 = new(STD_ROOM);
    object npc = new(STD_NPC);
    object item = new(STD_ITEM);

    r->set_short("A Test Room");
    r->set_long("A long description of the test room for wrapping.");
    r->set_long_footer("A footer line.");
    r->set_exit("north", "/domain/Nowhere/room/void.c");
    r->set_listen("default", (: "You hear a function." :));
    r->set_smell("default", "You smell a string.");
    npc->set_name("goblin");
    npc->handle_move(r);
    item->handle_move(r);

    testOb->set_name("Describer");
    testOb->set_user(this_object());

    // no environment yet: the early-return branch
    testOb->describe_environment();

    testOb->handle_move(r);
    // screenreader off: renders the room map beside the long description
    __ScreenReader = "off";
    testOb->describe_environment();
    // screenreader on: plain long description, no map
    __ScreenReader = "on";
    testOb->describe_environment();
    // immortal: adds the property annotation line
    testOb->set_immortal(1);
    testOb->describe_environment();
    testOb->set_immortal(0);

    // empty room: the "no exits" and empty-contents paths
    testOb->handle_move(r2);
    testOb->describe_environment();

    expect("describe_environment produced output", (: ({
        assert_equal(sizeof(__Received) > 0, 1),
    }) :));

    rmdir("/realm/describer");
    if (npc) destruct(npc);
    if (item) destruct(item);
    if (r) destruct(r);
    if (r2) destruct(r2);
}
