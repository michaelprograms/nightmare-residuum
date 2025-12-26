inherit M_TEST;

/**
 * @var {"/std/character"} testOb
 */

void test_npc () {
    expect("is_character behaves", (: ({
        assert_equal(livingp(testOb), 1),
        assert_equal(characterp(testOb), 1),
        assert_equal(npcp(testOb), 0),
    }) :));
}

void test_user () {
    expect("user is settable and queryable", (: ({
        assert_equal(testOb->query_user(), UNDEFINED),
        testOb->set_user(this_object()),
        assert_equal(testOb->query_user(), this_object()),
        testOb->set_user(0),
        assert_equal(testOb->query_user(), 0),
    }) :));
}

void test_account () {
    expect("account is settable and queryable", (: ({
        assert_equal(testOb->query_account(), UNDEFINED),
        testOb->set_account("test account"),
        assert_equal(testOb->query_account(), "test account"),
        testOb->set_account(0),
        assert_equal(testOb->query_account(), 0),
    }) :));
}

void test_times () {
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

void test_name () {
    expect("name handles character setup", (: ({
        assert_equal(testOb->query_name(), UNDEFINED),

        testOb->set_name("testcharacter"),
        assert_equal(testOb->query_name(), "testcharacter"),
        assert_equal(testOb->query_key_name(), "testcharacter"),
        assert_equal(testOb->query_cap_name(), "Testcharacter"),
        assert_equal(testOb->query_short(), "Testcharacter the character"),
        assert_equal(testOb->query_long(), "Testcharacter is an unknown."),
        assert_equal(testOb->query_save_path(), "/save/character/t/testcharacter/character.o"),
    }) :));
}

void test_immortal () {
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
void test_gmcp () {
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
        assert_equal(gmcpData, ([ "hp": 22, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 12, "sp": 12 ])),

        assert_equal(gmcpData = UNDEFINED, UNDEFINED),
        // vitals functions call gmcp
        testOb->set_hp(5),
        assert_equal(gmcpData, ([ "hp": 5, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 12, "sp": 12 ])),
        testOb->add_hp(5),
        assert_equal(gmcpData, ([ "hp": 10, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 12, "sp": 12 ])),

        testOb->set_sp(5),
        assert_equal(gmcpData, ([ "hp": 10, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 12, "sp": 5 ])),
        testOb->add_sp(5),
        assert_equal(gmcpData, ([ "hp": 10, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 12, "sp": 10 ])),

        testOb->set_mp(5),
        assert_equal(gmcpData, ([ "hp": 10, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 5, "sp": 10 ])),
        testOb->add_mp(5),
        assert_equal(gmcpData, ([ "hp": 10, "maxhp": 22, "maxmp": 12, "maxsp": 12, "mp": 10, "sp": 10 ])),
    }) :));
}

void test_go () {
    object r1 = new(STD_ROOM);
    object r2 = new(STD_ROOM);

    r1->set_exit("east", file_name(r2));
    r2->set_exit("west", file_name(r1));

    testOb->set_name("Test");
    testOb->set_user(this_object());

    expect("character can go", (: ({
        assert_equal(testOb->handle_move($(r1)), 1),
        assert_equal(testOb->handle_go(base_name($(r2)), "walk", "east", ), 1),
    }) :));

    if (r1) destruct(r1);
    if (r2) destruct(r2);
}