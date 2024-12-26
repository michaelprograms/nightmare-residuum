inherit M_TEST;

/**
 * @var {"/std/character"} testOb
 */

void test_npc () {
    expect("is_character behaves", (: ({
        assert_equal(testOb->is_living(), 1),
        assert_equal(testOb->is_character(), 1),
        assert_equal(testOb->is_npc(), UNDEFINED),
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